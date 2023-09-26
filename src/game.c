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

static bool check_death(t_player *player, const char *board)
{
    return false;
    // int x = player->x;
    // int y = player->y;
    // int nb_opponent = 0;
    // for (int i = x - 1; i <= x + 1; i++)
    // {
    //     for (int j = y - 1; j <= y + 1; j++)
    //     {
    //         if (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE)
    //         {
    //             if (board[i * BOARD_SIZE + j] != FREE_TILE && board[i * BOARD_SIZE + j] != (int)player->team + '0')
    //                 nb_opponent++;
    //         }
    //     }
    // }
    // if (nb_opponent >= NB_OPPONENT_TO_DIE)
    //     player->alive = false;
    // return (nb_opponent & NB_OPPONENT_TO_DIE);
}

static bool check_game_end(const t_game *game)
{
    // int team = -1;
    // for (size_t i = 0; i < game->nb_player; i++)
    // {
    //     if (game->player[i].alive)
    //     {
    //         if (team != game->player[i].team && team != -1)
    //             return true;
    //         team = game->player[i].team;
    //     }
    // }
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
        ft_printf("Setting up new target\n");
        search_new_target(board, p_coord, t_info);
        // check return
    }
    else
    {
        ft_printf("Searching target\n");
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
    ft_printf("p:%i %i\n", p_coord[LINE], p_coord[ROW]);
    if (direction)
        move_player(board, p_coord, direction);
    ft_printf("p:%i %i\n", p_coord[LINE], p_coord[ROW]);
}

void start_game(t_ipc *ipc)
{
    t_msg msg;
    msg.msg_type = ipc->player->team;
    ft_memset(&msg.t_info, -1, sizeof(int) * 3);
    int dest_coord[2];
    ft_memset(dest_coord, -1, sizeof(int) * 2);

    sem_lock(ipc->semid[GAME_MUTEX], LOCK);
    ft_printf("Starting game...team:%c\n", ipc->player->team + ASCII_OFFSET);
    // Check around player if dead or not
    // if (check_death(&ipc->game->player[ipc->player_id], ipc->game->board) || check_game_end(ipc->game))
    // {
    //     leave_game(ipc);
    //     semaphore_lock(ipc->sem[GAME_MUTEX], UNLOCK);
    //     exit(0);
    // }
    recv_msq(ipc->msqid, &msg, ipc->player->team);
    set_target(ipc->game->board, ipc->player->coord, msg.t_info);
    print_board(ipc->game->board);
    move_to_target(ipc->game->board, ipc->player->coord, msg.t_info);
    // send target in msq
    print_board(ipc->game->board);
    sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
}