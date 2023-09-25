#include "../inc/lemipc.h"

int init_game(t_ipc *ipc)
{
    int err;
    if ((err = init_sem(&ipc->sem[GAME_MUTEX], SEM_GAME_MUTEX_KEY, 1)))
        return err;
    if ((err = init_sem(&ipc->sem[WAITING_START], SEM_WAITING_GAME_KEY, 0)))
        return err;

    semaphore_lock(ipc->sem[GAME_MUTEX], LOCK);
    ft_printf("Initializing game...\n");
    ipc->game->started = false;
    ipc->player_id = 0;
    ipc->game->nb_player = 1;
    ft_memset(ipc->game->board, FREE_TILE, BOARD_SIZE * BOARD_SIZE);
    set_player_spawn(ipc->game);
    semaphore_lock(ipc->sem[GAME_MUTEX], UNLOCK);
    ft_printf("Sleeping admin...\n");
    sleep(SECOND_BEFORE_START);
    ft_printf("Waking up admin...\n");
    semaphore_lock(ipc->sem[GAME_MUTEX], LOCK);
    ipc->game->started = true;
    set_players_team(ipc);
    // print_board(ipc->game->board);
    for (size_t i = 0; i < ipc->game->nb_player - 1; i++)
        semaphore_lock(ipc->sem[WAITING_START], UNLOCK);

    semaphore_lock(ipc->sem[GAME_MUTEX], UNLOCK);
    return SUCCESS;
}

int add_player(t_ipc *ipc)
{
    int err;
    if ((err = init_sem(&ipc->sem[GAME_MUTEX], SEM_GAME_MUTEX_KEY, 1)))
        return err;
    if ((err = init_sem(&ipc->sem[WAITING_START], SEM_WAITING_GAME_KEY, 0)))
        return err;

    semaphore_lock(ipc->sem[GAME_MUTEX], LOCK);
    if (ipc->game->started)
    {
        semaphore_lock(ipc->sem[GAME_MUTEX], UNLOCK);
        return GAME_STARTED;
    }
    ipc->player_id = ipc->game->nb_player;
    ipc->game->nb_player++;
    set_player_spawn(ipc->game);
    semaphore_lock(ipc->sem[GAME_MUTEX], UNLOCK);
    semaphore_lock(ipc->sem[WAITING_START], LOCK);
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
    game->player[game->nb_player - 1].alive = true;
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