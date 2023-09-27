#include "../inc/lemipc.h"

void set_player_spawn(t_ipc *ipc)
{
    long line, row;
    char complete_line[BOARD_SIZE + 1] = {0};
    srand(time(NULL));
    int fd = open("/dev/random", O_RDONLY);
    char buff[1];
    do
    {
        read(fd, buff, 1);
        // Check open/read/close return...
        line = buff[0] % BOARD_SIZE;

        ft_memcpy(complete_line, &(ipc->game->board[line * BOARD_SIZE]), BOARD_SIZE);
    } while (!ft_strchr(complete_line, FREE_TILE));
    do
    {
        read(fd, buff, 1);
        row = buff[0] % BOARD_SIZE;
    } while (complete_line[row] != FREE_TILE);
    close(fd);
    ipc->game->board[line * BOARD_SIZE + row] = ipc->player->team + ASCII_OFFSET;
    ipc->player->coord[LINE] = line;
    ipc->player->coord[ROW] = row;
}

int add_player(t_ipc *ipc)
{
    sem_lock(ipc->semid[GAME_MUTEX], LOCK);
    if (ipc->game->started)
    {
        sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
        return GAME_STARTED;
    }
    set_player_spawn(ipc);
    sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
    sem_lock(ipc->semid[WAITING_START_MUTEX], LOCK);
    return SUCCESS;
}

void move_player(char *board, int p_coord[2], const int direction)
{
    switch (direction)
    {
    case UP:
        board[TOP_TILE] = board[CURRENT_TILE];
        board[CURRENT_TILE] = FREE_TILE;
        p_coord[LINE]--;
        break;
    case DOWN:
        board[BOTTOM_TILE] = board[CURRENT_TILE];
        board[CURRENT_TILE] = FREE_TILE;
        p_coord[LINE]++;
        break;
    case LEFT:
        board[LEFT_TILE] = board[CURRENT_TILE];
        board[CURRENT_TILE] = FREE_TILE;
        p_coord[ROW]--;
        break;
    case RIGHT:
        board[RIGHT_TILE] = board[CURRENT_TILE];
        board[CURRENT_TILE] = FREE_TILE;
        p_coord[ROW]++;
        break;
    }
}