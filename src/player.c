#include "../inc/lemipc.h"

int init_game(t_ipc *ipc)
{
    int err;
    if ((err = get_semaphore(ipc)))
        return err;

    sem_operation(ipc->sem.id, GAME_OPERATION, DECREMENT);
    ipc->game->started = false;
    ipc->game->nb_player = 1;
    ft_memset(ipc->game->board, FREE_TILE, BOARD_SIZE * BOARD_SIZE);
    set_player_spawn(ipc->game);
    sem_operation(ipc->sem.id, GAME_OPERATION, INCREMENT);
    ft_printf("Waiting for other players...\n");
    sleep(SECOND_BEFORE_START);
    // Have to quit if less than 4 players ?
    // + Choose number of players per team
    sem_operation(ipc->sem.id, GAME_OPERATION, DECREMENT);
    set_players_team(ipc);
    print_board(ipc->game->board);
    ipc->game->started = true;
    ft_printf("Starting...\n");
    // for (size_t i = 0; i < ipc->game->nb_player - 1; i++)
    // sem_operation(ipc->sem.id, WAITING_START, INCREMENT);
    sem_operation(ipc->sem.id, GAME_OPERATION, INCREMENT);
    return SUCCESS;
}

int add_player(t_ipc *ipc)
{
    int err;
    if ((err = get_semaphore(ipc)))
        return err;

    sem_operation(ipc->sem.id, GAME_OPERATION, DECREMENT);
    if (ipc->game->started)
    {
        sem_operation(ipc->sem.id, GAME_OPERATION, INCREMENT);
        return GAME_STARTED;
    }
    ipc->game->nb_player++;
    set_player_spawn(ipc->game);
    sem_operation(ipc->sem.id, GAME_OPERATION, INCREMENT);
    // sem_operation(ipc->sem.id, WAITING_START, DECREMENT);
    return SUCCESS;
}

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