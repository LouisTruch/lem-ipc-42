#include "../inc/lemipc.h"

int init_game(t_ipc *ipc)
{
    sem_lock(ipc->semid[GAME_MUTEX], LOCK);
    ft_printf("Initializing game...\n");
    ipc->game->started = false;
    ft_memset(ipc->game->board, FREE_TILE, BOARD_SIZE * BOARD_SIZE);
    sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
    sleep(TIME_BEFORE_START);
    sem_lock(ipc->semid[GAME_MUTEX], LOCK);
    set_player_spawn(ipc);
    struct shmid_ds shmid_ds;
    if (get_shmem_stat(ipc->shmid, &shmid_ds) == IPC_ERROR)
    {
        perror("game.c init_game(): shmctl");
        return (SHMCTL_STAT_ERROR);
    }
    ft_printf("Starting game with %i players..\n", shmid_ds.shm_nattch);
    for (size_t i = 0; i < shmid_ds.shm_nattch - 1; i++)
        sem_lock(ipc->semid[WAITING_START_MUTEX], UNLOCK);
    ipc->game->started = true;
    sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
    return SUCCESS;
}

static void leave_game(t_game *game, int p_coord[2], int semid)
{
    game->board[CURRENT_TILE] = FREE_TILE;
    shmdt(game);
    sem_lock(semid, UNLOCK);
    exit(SUCCESS);
}

static bool check_player_death(t_player *player, const char *board)
{
    char p_team = player->team + ASCII_OFFSET;
    int nb_ennemies[NB_TEAM] = {0};
    for (int line = player->coord[LINE] - 1; line <= player->coord[LINE] + 1; line++)
        for (int row = player->coord[ROW] - 1; row <= player->coord[ROW] + 1; row++)
            if (line >= 0 && line < BOARD_SIZE && row >= 0 && row < BOARD_SIZE)
                if (board[line * BOARD_SIZE + row] != FREE_TILE && board[line * BOARD_SIZE + row] != p_team)
                {
                    // Bon...
                    nb_ennemies[board[line * BOARD_SIZE + row] - ASCII_OFFSET]++;
                    if (nb_ennemies[board[line * BOARD_SIZE + row] - ASCII_OFFSET] >= NB_OPPONENT_TO_DIE)
                        return true;
                }
    return false;
}

static bool check_game_win(const char *board, const int team)
{
    int nb_player = 0;
    int p_team = team;
    for (int i = 0; board[i]; i++)
        if (board[i] != FREE_TILE)
        {
            nb_player++;
            if (board[i] != p_team + ASCII_OFFSET)
                p_team++;
        }
    if (nb_player <= 2 || p_team == team)
        return true;
    return false;
}

static bool check_draw(const char *board)
{
    int nb_ennemies[NB_TEAM] = {0};
    for (int i = 0; board[i]; i++)
        if (board[i] != FREE_TILE)
            nb_ennemies[board[i] - ASCII_OFFSET]++;
    for (int i = 0; i < NB_TEAM; i++)
        if (nb_ennemies[i] > 1)
            return false;
    return true;
}

static bool search_new_target(const char *board, int p_coord[2], int t_info[3])
{
    // ...
    char p_team = board[p_coord[LINE] * BOARD_SIZE + p_coord[ROW]];
    for (int sqr_size = 1; sqr_size <= (BOARD_SIZE + 1) * 2; sqr_size++)
        for (int line = p_coord[LINE] - sqr_size; line <= p_coord[LINE] + sqr_size; line++)
            for (int row = p_coord[ROW] - sqr_size; row <= p_coord[ROW] + sqr_size; row++)
                if (line >= 0 && line < BOARD_SIZE && row >= 0 && row < BOARD_SIZE)
                    if (board[line * BOARD_SIZE + row] != FREE_TILE && board[line * BOARD_SIZE + row] != p_team)
                    {
                        // ft_printf("my team%c found ennemy in %i %i\n", p_team, i, j);
                        t_info[LINE] = line;
                        t_info[ROW] = row;
                        t_info[TEAM] = board[line * BOARD_SIZE + row] - ASCII_OFFSET;
                        return true;
                    }
    return false;
}

static bool search_target(const char *board, int t_info[3])
{
    // f;afafdff
    if (board[t_info[LINE] * BOARD_SIZE + t_info[ROW]] == t_info[TEAM] + ASCII_OFFSET)
        return true;
    if (board[(t_info[LINE] - 1) * BOARD_SIZE + t_info[ROW]] &&
        board[(t_info[LINE] - 1) * BOARD_SIZE + t_info[ROW]] == t_info[TEAM] + ASCII_OFFSET)
    {
        t_info[LINE]--;
        return true;
    }
    if (board[(t_info[LINE] + 1) * BOARD_SIZE + t_info[ROW]] &&
        board[(t_info[LINE] + 1) * BOARD_SIZE + t_info[ROW]] == t_info[TEAM] + ASCII_OFFSET)
    {
        t_info[LINE]++;
        return true;
    }
    if (board[(t_info[LINE]) * BOARD_SIZE + t_info[ROW] - 1] &&
        board[(t_info[LINE]) * BOARD_SIZE + t_info[ROW] - 1] == t_info[TEAM] + ASCII_OFFSET)
    {
        t_info[ROW]--;
        return true;
    }
    if (board[(t_info[LINE]) * BOARD_SIZE + t_info[ROW] + 1] &&
        board[(t_info[LINE]) * BOARD_SIZE + t_info[ROW] + 1] == t_info[TEAM] + ASCII_OFFSET)
    {
        t_info[ROW]++;
        return true;
    }
    return false;
}

void set_target(const char *board, int p_coord[2], int t_info[3])
{
    if (t_info[TEAM] == -1)
    {
        search_new_target(board, p_coord, t_info);
    }
    else
    {
        if (!search_target(board, t_info))
            search_new_target(board, p_coord, t_info);
    }
}

static bool is_around_target(const char *board, const int p_coord[2], const int t_info[3])
{
    for (int line = p_coord[LINE] - 1; line <= p_coord[LINE] + 1; line++)
    {
        for (int row = p_coord[ROW] - 1; row <= p_coord[ROW] + 1; row++)
        {
            if (line >= 0 && line < BOARD_SIZE && row >= 0 && row < BOARD_SIZE)
            {
                if (board[line * BOARD_SIZE + row] == t_info[TEAM] + ASCII_OFFSET)
                    return true;
            }
        }
    }
    return false;
}

static bool is_p_around_target(const char *board, const int t_info[3])
{
    for (int line = t_info[LINE] - 1; line <= t_info[LINE] + 1; line++)
    {
        for (int row = t_info[ROW] - 1; row <= t_info[ROW] + 1; row++)
        {
            if (line >= 0 && line < BOARD_SIZE && row >= 0 && row < BOARD_SIZE)
            {
                if (board[line * BOARD_SIZE + row] == 'P')
                    return true;
            }
        }
    }
    return false;
}

static void flood_fill(char *c_board)
{
    for (int line = 0; line < BOARD_SIZE; line++)
    {
        for (int row = 0; row < BOARD_SIZE; row++)
        {
            if (c_board[line * BOARD_SIZE + row] == 'P')
            {
                if (line - 1 >= 0 && c_board[(line - 1) * BOARD_SIZE + row] == FREE_TILE)
                    c_board[(line - 1) * BOARD_SIZE + row] = 'p';
                if (line + 1 < BOARD_SIZE && c_board[(line + 1) * BOARD_SIZE + row] == FREE_TILE)
                    c_board[(line + 1) * BOARD_SIZE + row] = 'p';
                if (row - 1 >= 0 && c_board[line * BOARD_SIZE + row - 1] == FREE_TILE)
                    c_board[line * BOARD_SIZE + row - 1] = 'p';
                if (row + 1 < BOARD_SIZE && c_board[line * BOARD_SIZE + row + 1] == FREE_TILE)
                    c_board[line * BOARD_SIZE + row + 1] = 'p';
            }
        }
    }
    for (int i = 0; c_board[i]; i++)
    {
        if (c_board[i] == 'p')
            c_board[i] = 'P';
    }
}

static int find_path_length(const char *board, const int p_coord[2], const int t_info[3])
{
    if (p_coord[LINE] < 0 || p_coord[LINE] >= BOARD_SIZE || p_coord[ROW] < 0 || p_coord[ROW] >= BOARD_SIZE)
        return NO_PATH;

    char c_board[BOARD_SIZE * BOARD_SIZE];
    ft_memcpy(c_board, board, BOARD_SIZE * BOARD_SIZE);
    c_board[(p_coord[LINE]) * BOARD_SIZE + p_coord[ROW]] = 'p';
    c_board[TOP_TILE] = 'p';
    int path_length = 0;
    while (!is_p_around_target(c_board, t_info))
    {
        flood_fill(c_board);
        path_length++;
        if (path_length > BOARD_SIZE * BOARD_SIZE)
            return NO_PATH;
    }
    return path_length;
}

static int select_path(const char *board, const int p_coord[2], const int t_info[3])
{
    if (is_around_target(board, p_coord, t_info))
        return CANT_MOVE;

    // Check map limit here
    int path_length[4];
    path_length[UP] = find_path_length(board, (int[2]){p_coord[LINE] - 1, p_coord[ROW]}, t_info);
    path_length[DOWN] = find_path_length(board, (int[2]){p_coord[LINE] + 1, p_coord[ROW]}, t_info);
    path_length[LEFT] = find_path_length(board, (int[2]){p_coord[LINE], p_coord[ROW] - 1}, t_info);
    path_length[RIGHT] = find_path_length(board, (int[2]){p_coord[LINE], p_coord[ROW] + 1}, t_info);

    int min_length = INT_MAX;
    int min_index;
    for (int i = 0; i < 4; i++)
    {
        if (path_length[i] >= 0 && path_length[i] < min_length)
        {
            min_length = path_length[i];
            min_index = i;
        }
    }
    return min_index != INT_MAX ? min_index : CANT_MOVE;
}

static bool check_end_conds(t_ipc *ipc)
{
    if (check_player_death(ipc->player, ipc->game->board))
    {
        // ft_printf("Leaving: Death\n");
        return true;
    }
    if (check_game_win(ipc->game->board, ipc->player->team))
    {
        // ft_printf("Leaving: Game ended\n");
        return true;
    }
    if (check_draw(ipc->game->board))
    {
        // ft_printf("Leaving: Draw\n");
        return true;
    }
    return false;
}

void send_board(const int msqid, const char *board)
{
    t_msg_spect msg;
    msg.mtype = 1;
    ft_memcpy(msg.mtext, board, BOARD_SIZE * BOARD_SIZE);
    if (msgsnd(msqid, &msg, (sizeof(t_msg_spect) - sizeof(long)), 0) == IPC_ERROR)
    {
        perror("msgsnd");
    }
}

void start_game(t_ipc *ipc)
{
    t_msg msg;
    ft_memset(&msg.mtext, -1, sizeof(int) * 3);
    msg.mtype = ipc->player->team;

    while (1)
    {
        sem_lock(ipc->semid[GAME_MUTEX], LOCK);
        recv_msq(ipc->msqid[PLAY], &msg, ipc->player->team);
        if (check_end_conds(ipc))
            break;
        // sem_lock(ipc->semid[SPECTATE_MUTEX], LOCK);
        set_target(ipc->game->board, ipc->player->coord, msg.mtext);
        int direction = select_path(ipc->game->board, ipc->player->coord, msg.mtext);
        move_player(ipc->game->board, ipc->player->coord, direction);
        send_board(ipc->msqid[SPECT], ipc->game->board);
        usleep(GAME_SPEED);
        // sem_lock(ipc->semid[SPECTATE_MUTEX], UNLOCK);
        send_msq(ipc->msqid[PLAY], &msg);
        sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
    }
    // ft_printf("Leaving game...\n");
    leave_game(ipc->game, ipc->player->coord, ipc->semid[GAME_MUTEX]);
}

static bool board_empty(t_ipc *ipc)
{
    for (int i = 0; ipc->game->board[i]; i++)
    {
        if (ipc->game->board[i] != FREE_TILE)
        {
            return false;
        }
    }
    return true;
}

void start_spectating(t_ipc *ipc)
{
    while (!ipc->game->started)
    {
        sem_lock(ipc->semid[GAME_MUTEX], LOCK);
        // usleep(GAME_SPEED);
        sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
    }
    ft_printf("Starting spectator mode\n");
    while (1)
    {
        sem_lock(ipc->semid[SPECTATE_MUTEX], LOCK);
        if (board_empty(ipc))
        {
            ft_printf("Exiting spectator mode\n");
            sem_lock(ipc->semid[SPECTATE_MUTEX], UNLOCK);
            return;
        }
        print_board(ipc->game->board);
        sem_lock(ipc->semid[SPECTATE_MUTEX], UNLOCK);
    }
}