#include "../inc/lemipc.h"

int init_game(t_ipc *ipc)
{
    sem_lock(ipc->semid[GAME_MUTEX], LOCK);
    ft_printf("Initializing game...\n");
    ipc->game->started = false;
    ft_memset(ipc->game->board, FREE_TILE, BOARD_SIZE * BOARD_SIZE);
    sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
    ft_printf("Sleeping admin...\n");
    sleep(SECOND_BEFORE_START);
    ft_printf("Waking up admin...\n");
    // Change this 2 lines below
    sem_lock(ipc->semid[GAME_MUTEX], LOCK);
    set_player_spawn(ipc);
    struct shmid_ds shmid_ds;
    if (get_shmem_stat(ipc->shmid, &shmid_ds) == IPC_ERROR)
    {
        perror("game.c init_game(): shmctl");
        return (SHMCTL_STAT_ERROR);
    }
    for (size_t i = 0; i < shmid_ds.shm_nattch - 1; i++)
        sem_lock(ipc->semid[WAITING_START_MUTEX], UNLOCK);
    ipc->game->started = true;
    sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
    return SUCCESS;
}

static void leave_game(t_game *game, int p_coord[2])
{
    game->board[CURRENT_TILE] = FREE_TILE;
    shmdt(game);
}

static bool check_player_death(t_player *player, const char *board)
{
    char p_team = player->team + ASCII_OFFSET;
    int nb_ennemies[NB_TEAM] = {0};
    for (int line = player->coord[LINE] - 1; line <= player->coord[LINE] + 1; line++)
    {
        for (int row = player->coord[ROW] - 1; row <= player->coord[ROW] + 1; row++)
        {
            if (line >= 0 && line < BOARD_SIZE && row >= 0 && row < BOARD_SIZE)
            {
                if (board[line * BOARD_SIZE + row] != FREE_TILE && board[line * BOARD_SIZE + row] != p_team)
                {
                    // Bon...
                    nb_ennemies[board[line * BOARD_SIZE + row] - ASCII_OFFSET]++;
                    if (nb_ennemies[board[line * BOARD_SIZE + row] - ASCII_OFFSET]++ >= NB_OPPONENT_TO_DIE)
                        return true;
                }
            }
        }
    }

    return false;
}

static bool check_game_end(const char *board, const int team)
{
    int nb_player = 0;
    int p_team = team;
    for (int i = 0; board[i]; i++)
    {
        if (board[i] != FREE_TILE)
        {
            nb_player++;
            if (board[i] != p_team + ASCII_OFFSET)
                p_team++;
        }
    }
    if (nb_player <= 2 || p_team == team)
        return true;
    return false;
}

static bool search_new_target(const char *board, int p_coord[2], int t_info[3])
{
    // ...
    char p_team = board[p_coord[LINE] * BOARD_SIZE + p_coord[ROW]];
    for (int sqr_size = 1; sqr_size <= BOARD_SIZE; sqr_size++)
    {
        for (int line = p_coord[LINE] - sqr_size; line <= p_coord[LINE] + sqr_size; line++)
        {
            for (int row = p_coord[ROW] - sqr_size; row <= p_coord[ROW] + sqr_size; row++)
            {
                if (line >= 0 && line < BOARD_SIZE && row >= 0 && row < BOARD_SIZE)
                {
                    if (board[line * BOARD_SIZE + row] != FREE_TILE && board[line * BOARD_SIZE + row] != p_team)
                    {
                        // ft_printf("my team%c found ennemy in %i %i\n", p_team, i, j);
                        t_info[LINE] = line;
                        t_info[ROW] = row;
                        t_info[TEAM] = board[line * BOARD_SIZE + row] - ASCII_OFFSET;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

static bool search_target(const char *board, int t_info[3])
{
    // ...
    for (int sqr_size = 1; sqr_size <= BOARD_SIZE; sqr_size++)
    {
        for (int line = t_info[LINE] - sqr_size; line <= t_info[LINE] + sqr_size; line++)
        {
            for (int row = t_info[ROW] - sqr_size; row <= t_info[ROW] + sqr_size; row++)
            {
                if (line >= 0 && line < BOARD_SIZE && row >= 0 && row < BOARD_SIZE)
                {
                    if (board[line * BOARD_SIZE + row] == t_info[TEAM] + ASCII_OFFSET)
                    {
                        // ft_printf("my team%c found ennemy in %i %i\n", p_team, i, j);
                        t_info[LINE] = line;
                        t_info[ROW] = row;
                        return true;
                    }
                }
            }
        }
    }
    // Probably find new target if can't find target
    return false;
}

static void set_target(const char *board, int p_coord[2], int t_info[3])
{
    if (t_info[TEAM] == -1)
    {
        search_new_target(board, p_coord, t_info);
        // check return
    }
    else
    {
        if (!search_target(board, t_info))
            search_new_target(board, p_coord, t_info);
        // ft_printf("Found target %c at %i %i", t_info[TEAM] + ASCII_OFFSET, t_info[LINE], t_info[ROW]);
    }
}

static bool is_tile_free(const char tile)
{
    if (tile == FREE_TILE)
        return true;
    return false;
}

static int select_direction(const char *board, const int p_coord[2], const int t_info[3])
{
    // ft_printf("t:%i %i\n", t_info[LINE], t_info[ROW]);
    if (t_info[LINE] == -1)
        return CANT_MOVE;
    if (t_info[LINE] < p_coord[LINE] && is_tile_free(board[BOTTOM_TILE]))
        return UP;
    if (t_info[LINE] > p_coord[LINE] && is_tile_free(board[TOP_TILE]))
        return DOWN;
    if (t_info[ROW] < p_coord[ROW] && is_tile_free(board[LEFT_TILE]))
        return LEFT;
    if (t_info[ROW] > p_coord[ROW] && is_tile_free(board[RIGHT_TILE]))
        return RIGHT;

    // Add random move, dont move only if surrounded by 4 players
    return CANT_MOVE;
}

static void move_to_target(char *board, int p_coord[2], const int t_info[3])
{
    int direction = select_direction(board, p_coord, t_info);
    // ft_printf("p:%i %i\n", p_coord[LINE], p_coord[ROW]);
    if (direction)
        move_player(board, p_coord, direction);
    // ft_printf("p:%i %i\n", p_coord[LINE], p_coord[ROW]);
}

void start_game(t_ipc *ipc)
{
    t_msg msg;
    msg.msg_type = ipc->player->team;
    ft_memset(&msg.t_info, -1, sizeof(int) * 3);

    for (int i = 0; i < 10; i++)
    {
        sem_lock(ipc->semid[GAME_MUTEX], LOCK);
        // ft_printf("Starting game...team:%c\n", ipc->player->team + ASCII_OFFSET);
        // Check around player if dead or not
        if (check_player_death(ipc->player, ipc->game->board) || check_game_end(ipc->game->board, ipc->player->team))
        {
            ft_printf("Leaving game\n");
            leave_game(ipc->game, ipc->player->coord);
            // Change exit value
            sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
            exit(0);
        }
        recv_msq(ipc->msqid, &msg, ipc->player->team);
        set_target(ipc->game->board, ipc->player->coord, msg.t_info);
        move_to_target(ipc->game->board, ipc->player->coord, msg.t_info);
        send_msq(ipc->msqid, &msg);
        print_board(ipc->game->board);
        sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
        usleep(GAME_SPEED);
    }
}