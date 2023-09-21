#include "../inc/lemipc.h"

int add_player(t_ipc *ipc)
{
    ipc->game->nb_player++;
    ipc->game->player[ipc->game->nb_player - 1].x = 9;
    ipc->game->player[ipc->game->nb_player - 1].y = 9;
    ipc->game->player[ipc->game->nb_player - 1].team = 1;

    int err;
    if ((err = get_semaphore(ipc)))
        return err;

    sem_operation(ipc->sem.id, DECREMENT);
    print_board(ipc->game->board);
    sem_operation(ipc->sem.id, INCREMENT);

    return (0);
}

int init_game(t_ipc *ipc)
{
    int err;
    if ((err = get_semaphore(ipc)))
        return err;

    sem_operation(ipc->sem.id, DECREMENT);
    ipc->game->nb_player = 1;
    ipc->game->player[0].x = 2;
    ipc->game->player[0].y = 2;
    ipc->game->player[0].team = 0;

    ft_memset(ipc->game->board, 'x', BOARD_SIZE * BOARD_SIZE);
    print_board(ipc->game->board);

    sem_operation(ipc->sem.id, INCREMENT);

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

    sem_operation(ipc.sem.id, DECREMENT);
    ft_printf("sleeping..\n");
#include <unistd.h>
    sleep(8);
    ft_printf("waking up..\n");

    sem_operation(ipc.sem.id, INCREMENT);

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