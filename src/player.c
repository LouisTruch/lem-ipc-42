#include "../inc/lemipc.h"

void set_player_spawn(t_ipc *ipc)
{
    int i_line, i_row;
    char line[BOARD_SIZE];
    // ft_memset(line, 0, BOARD_SIZE);
    while (1)
    {
        i_line = rand() % BOARD_SIZE;
        ft_memcpy(line, &ipc->game->board[i_line * BOARD_SIZE], BOARD_SIZE);
        if (ft_strchr(line, FREE_TILE))
        {
            while (1)
            {
                i_row = rand() % BOARD_SIZE;
                if (line[i_row] == FREE_TILE)
                    break;
            }
            break;
        }
    }
    ft_printf("Spawn set team:%i %i %i\n", ipc->player->team, i_line, i_row);
    ipc->game->board[i_line * BOARD_SIZE + i_row] = ipc->player->team + ASCII_OFFSET;
    ipc->player->coord[LINE] = i_line;
    ipc->player->coord[ROW] = i_row;
}

void add_player(t_ipc *ipc)
{
    sem_lock(ipc->semid[GAME_MUTEX], LOCK);
    if (ipc->game->started)
    {
        ft_putstr_fd("Game already started, exiting...\n", STDERR_FILENO);
        sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
        exit(GAME_STARTED);
    }
    set_player_spawn(ipc);
    sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
    sem_lock(ipc->semid[WAITING_START_MUTEX], LOCK);
}

void move_player(char *board, int p_coord[2], const int direction)
{
    switch (direction)
    {
    case UP:
        if (board[TOP_TILE] != FREE_TILE)
        {
            ft_printf("pb up '%c'\n", board[TOP_TILE]);
            return;
        }
        board[TOP_TILE] = board[CURRENT_TILE];
        board[CURRENT_TILE] = FREE_TILE;
        p_coord[LINE]--;
        break;
    case DOWN:
        if (board[BOTTOM_TILE] != FREE_TILE)
        {
            ft_printf("pb down '%c'\n", board[BOTTOM_TILE]);
            return;
        }
        board[BOTTOM_TILE] = board[CURRENT_TILE];
        board[CURRENT_TILE] = FREE_TILE;
        p_coord[LINE]++;
        break;
    case LEFT:
        if (board[LEFT_TILE] != FREE_TILE)
        {
            ft_printf("pb left '%c'\n", board[LEFT_TILE]);
            return;
        }
        board[LEFT_TILE] = board[CURRENT_TILE];
        board[CURRENT_TILE] = FREE_TILE;
        p_coord[ROW]--;
        break;
    case RIGHT:
        if (board[RIGHT_TILE] != FREE_TILE)
        {
            ft_printf("pb right '%c'\n", board[RIGHT_TILE]);
            return;
        }
        board[RIGHT_TILE] = board[CURRENT_TILE];
        board[CURRENT_TILE] = FREE_TILE;
        p_coord[ROW]++;
        break;
    }
}