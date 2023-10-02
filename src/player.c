#include "../inc/lemipc.h"

void set_player_spawn(t_ipc *ipc)
{
    errno = ERRNO_DEFAULT;
    int fd = open(URANDOM_PATH, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return;
    }
    int line, row;
    char complete_line[BOARD_SIZE + 1] = {0};
    char buff[1];
    do
    {
        if (read(fd, buff, 1) == -1)
        {
            perror("read");
            return;
        }
        // Check open/read/close return...
        line = buff[0] % BOARD_SIZE;
        ft_memcpy(complete_line, &(ipc->game->board[line * BOARD_SIZE]), BOARD_SIZE);
    } while (!ft_strchr(complete_line, FREE_TILE));
    do
    {
        if (read(fd, buff, 1) == -1)
        {
            perror("read");
            return;
        }
        row = buff[0] % BOARD_SIZE;
    } while (complete_line[row] != FREE_TILE);

    if (close(fd) == -1)
    {
        perror("close");
        return;
    }
    ft_printf("Spawn set team:%i\n", ipc->player->team);
    ipc->game->board[line * BOARD_SIZE + row] = ipc->player->team + ASCII_OFFSET;
    ipc->player->coord[LINE] = line;
    ipc->player->coord[ROW] = row;
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