#include "../inc/lemipc.h"

void set_player_spawn(t_game *game)
{
    long x, y;
    char line[BOARD_SIZE + 1] = {0};
    srandom(time(NULL));
    do
    {
        x = random() % BOARD_SIZE;
        ft_memcpy(line, &(game->board[x * BOARD_SIZE]), BOARD_SIZE);
    } while (!ft_strchr(line, FREE_TILE));
    do
    {
        y = random() % BOARD_SIZE;
    } while (line[y] != FREE_TILE);
    game->board[x * BOARD_SIZE + y] = '0';
    game->player[game->nb_player - 1].x = x;
    game->player[game->nb_player - 1].y = y;
}

void set_players_team(t_ipc *ipc)
{
    int x, y;
    for (size_t i = 0; i < ipc->game->nb_player; i++)
    {
        ipc->game->player[i].team = i % NB_TEAM;
        x = ipc->game->player[i].x;
        y = ipc->game->player[i].y;
        ipc->game->board[x * BOARD_SIZE + y] = ipc->game->player[i].team + '0';
    }
}