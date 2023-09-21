#include "../inc/lemipc.h"

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

    return (0);
}

int init_game(t_ipc *ipc)
{
    int err;
    if ((err = get_semaphore(ipc)))
        return err;

    sem_operation(ipc->sem.id, GAME_OPERATION, DECREMENT);
    sem_operation(ipc->sem.id, WAITING_START, DECREMENT);
    ipc->game->started = false;
    ipc->game->nb_player = 1;
    ft_memset(ipc->game->board, FREE_TILE, BOARD_SIZE * BOARD_SIZE);
    set_player_spawn(ipc->game);
    sem_operation(ipc->sem.id, GAME_OPERATION, INCREMENT);
    sleep(SECOND_BEFORE_START);
    // Have to quit if less than 4 players ?
    // + Choose number of players per team
    sem_operation(ipc->sem.id, GAME_OPERATION, DECREMENT);
    ipc->game->started = true;
    set_players_team(ipc);
    print_board(ipc->game->board);
    sem_operation(ipc->sem.id, GAME_OPERATION, INCREMENT);
    for (size_t i = 0; i < ipc->game->nb_player; i++)
        sem_operation(ipc->sem.id, WAITING_START, INCREMENT);
    return 0;
}

int main(int argc, char **argv)
{
    (void)argc;
    int err;
    t_ipc ipc;
    if ((err = init_shmem(FTOK_SHM_FILEPATH, &ipc)))
        return (err);
    if (ipc.first_player)
    {
        if ((err = init_game(&ipc)))
            return (err);
    }

    struct shmid_ds shmid_ds;
    if (get_shmem_stat(ipc.shm.id, &shmid_ds) == IPC_ERROR)
    {
        perror("shmctl");
        return (SHMCTL_STAT_ERROR);
    }
    printf("nb_player: %zu\n", shmid_ds.shm_nattch);

    if (!ipc.first_player)
    {
        ft_printf("Not FP\n");
        if ((err = add_player(&ipc)))
            return (err);
    }
    sem_operation(ipc.sem.id, WAITING_START, DECREMENT);
    ft_printf("game starting\n");

    if (destroy_shmem(ipc.shm.id) == IPC_ERROR)
    {
        perror("shmctl");
        return SHMCTL_RMID_ERROR;
    }

    if (destroy_semaphore(ipc.sem.id) == IPC_ERROR)
    {
        perror("semctl rmid");
        return SEMCTL_RM_ERROR;
    }

    return EXIT_SUCCESS;
}