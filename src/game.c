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
                    if (nb_ennemies[board[line * BOARD_SIZE + row] - ASCII_OFFSET] >= NB_OPPONENT_TO_DIE)
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

static bool check_draw(const char *board)
{
    int nb_ennemies[NB_TEAM] = {0};
    for (int i = 0; board[i]; i++)
    {
        if (board[i] != FREE_TILE)
            nb_ennemies[board[i] - ASCII_OFFSET]++;
    }
    for (int i = 0; i < NB_TEAM; i++)
    {
        if (nb_ennemies[i] > 1)
            return false;
    }
    return true;
}

static bool search_new_target(const char *board, int p_coord[2], int t_info[3])
{
    // ...
    char p_team = board[p_coord[LINE] * BOARD_SIZE + p_coord[ROW]];
    for (int sqr_size = 1; sqr_size <= (BOARD_SIZE + 1) * 2; sqr_size++)
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
    for (int sqr_size = 1; sqr_size <= (BOARD_SIZE + 1) * 2; sqr_size++)
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

void set_target(const char *board, int p_coord[2], int t_info[3])
{
    if (t_info[TEAM] == -1)
    {
        search_new_target(board, p_coord, t_info);
        ft_printf("Me '%c' %i %i Searched new target %i %i\n", board[CURRENT_TILE], p_coord[0], p_coord[1], t_info[0], t_info[1]);
        // check return
    }
    else
    {
        if (!search_target(board, t_info))
            search_new_target(board, p_coord, t_info);
        ft_printf("Me '%c' %i %i Target %i %i\n", board[CURRENT_TILE], p_coord[0], p_coord[1], t_info[0], t_info[1]);
        // ft_printf("Found target %c at %i %i", t_info[TEAM] + ASCII_OFFSET, t_info[LINE], t_info[ROW]);
    }
}

static int select_direction(const char *board, const int p_coord[2], const int t_info[3])
{
    // ff...
    if (p_coord[LINE] > t_info[LINE] && is_tile_free(board[TOP_TILE]))
        return UP;
    else if (p_coord[LINE] > t_info[LINE] && !is_tile_free(board[TOP_TILE]))
    {
        if (t_info[ROW] < p_coord[ROW] && is_tile_free(board[LEFT_TILE]))
            return LEFT;
        else if (t_info[ROW] > p_coord[ROW] && is_tile_free(board[RIGHT_TILE]))
            return RIGHT;
    }

    if (p_coord[LINE] < t_info[LINE] && is_tile_free(board[BOTTOM_TILE]))
        return DOWN;
    else if (p_coord[LINE] < t_info[LINE] && !is_tile_free(board[BOTTOM_TILE]))
    {
        if (t_info[ROW] < p_coord[ROW] && is_tile_free(board[LEFT_TILE]))
            return LEFT;
        else if (t_info[ROW] > p_coord[ROW] && is_tile_free(board[RIGHT_TILE]))
            return RIGHT;
    }

    if (t_info[ROW] < p_coord[ROW] && is_tile_free(board[LEFT_TILE]))
        return LEFT;
    else if (t_info[ROW] < p_coord[ROW] && !is_tile_free(board[LEFT_TILE]))
    {
        if (p_coord[LINE] > t_info[LINE] && is_tile_free(board[TOP_TILE]))
            return UP;
        else if (p_coord[LINE] < t_info[LINE] && is_tile_free(board[BOTTOM_TILE]))
            return DOWN;
    }

    if (t_info[ROW] > p_coord[ROW] && is_tile_free(board[RIGHT_TILE]))
        return RIGHT;
    else if (t_info[ROW] > p_coord[ROW] && !is_tile_free(board[RIGHT_TILE]))
    {
        if (p_coord[LINE] > t_info[LINE] && is_tile_free(board[TOP_TILE]))
            return UP;
        else if (p_coord[LINE] < t_info[LINE] && is_tile_free(board[BOTTOM_TILE]))
            return DOWN;
    }

    return (CANT_MOVE);
}

static void move_to_target(char *board, int p_coord[2], const int t_info[3])
{
    int direction = select_direction(board, p_coord, t_info);
    ft_printf("dir %i\n", direction);
    if (direction)
        move_player(board, p_coord, direction);
}

void start_game(t_ipc *ipc)
{
    t_msg msg;
    msg.msg_type = ipc->player->team;
    ft_memset(&msg.t_info, -1, sizeof(msg.t_info));
    for (int i = 0; i < 10; i++)
    {
        // ft_printf("Starting game...team:%c\n", ipc->player->team + ASCII_OFFSET);
        // Check around player if dead or not
        sem_lock(ipc->semid[GAME_MUTEX], LOCK);
        if (check_player_death(ipc->player, ipc->game->board) || check_game_end(ipc->game->board, ipc->player->team) || check_draw(ipc->game->board))
        {
            ft_printf("Leaving game\n");
            leave_game(ipc->game, ipc->player->coord);
            // Change exit value
            sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
            exit(0);
        }
        sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
        // struct msqid_ds msq_struct;
        // msgctl(ipc->msqid, IPC_STAT, &msq_struct);
        // ft_printf("stat %i %i\n", msq_struct.msg_qnum, msq_struct.msg_qbytes);
        // struct msginfo info;
        // msgctl(ipc->msqid, MSG_INFO, &info);
        // ft_printf("mess %i\n", info.msgtql);
        // ft_printf("recv_msg me=%c %i i=%i\n", ipc->player->team + ASCII_OFFSET, getpid(), i);
        recv_msq(ipc->msqid, &msg, ipc->player->team);
        sem_lock(ipc->semid[GAME_MUTEX], LOCK);
        set_target(ipc->game->board, ipc->player->coord, msg.t_info);
        move_to_target(ipc->game->board, ipc->player->coord, msg.t_info);
        sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
        send_msq(ipc->msqid, &msg);
        usleep(GAME_SPEED);
    }
}

void start_spectating(t_ipc *ipc)
{

    for (int i = 0; i < 20; i++)
    {
        while (!ipc->game->started)
        {
            sem_lock(ipc->semid[GAME_MUTEX], LOCK);
            usleep(GAME_SPEED);
            sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
        }

        sem_lock(ipc->semid[GAME_MUTEX], LOCK);
        print_board(ipc->game->board);
        usleep(GAME_SPEED);
        sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
    }
}