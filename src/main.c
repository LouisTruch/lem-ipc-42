#include "../inc/lemipc.h"

int clean_up_ipcs(t_ipc *ipc)
{
    struct shmid_ds shmid_ds;

    semaphore_lock(ipc->sem[GAME_MUTEX], LOCK);
    if (get_shmem_stat(ipc->shm, &shmid_ds) == IPC_ERROR)
    {
        perror("shmctl");
        return (SHMCTL_STAT_ERROR);
    }
    if (shmid_ds.shm_nattch > 1)
    {
        semaphore_lock(ipc->sem[GAME_MUTEX], UNLOCK);
        return 0;
    }
    semaphore_lock(ipc->sem[GAME_MUTEX], UNLOCK);

    if (destroy_shmem(ipc->shm) == IPC_ERROR)
    {
        perror("shmctl");
        return SHMCTL_RMID_ERROR;
    }
    if (destroy_semaphore(ipc->sem[GAME_MUTEX]) == IPC_ERROR)
    {
        perror("semctl rmid");
        return SEMCTL_RM_ERROR;
    }
    if (destroy_semaphore(ipc->sem[WAITING_START]) == IPC_ERROR)
    {
        perror("semctl rmid");
        return SEMCTL_RM_ERROR;
    }
    return SUCCESS;
}

int main(int argc, char **argv)
{
    (void)argc;
    int err;
    t_ipc ipc;
    if ((err = init_shmem(FTOK_SHM_FILEPATH, &ipc)))
        return err;
    if (ipc.first_player)
    {
        if ((err = init_game(&ipc)))
            return err;
    }
    else if (!ipc.first_player)
    {
        if ((err = add_player(&ipc)))
            return err;
    }
    start_playing(ipc);

    return (clean_up_ipcs(&ipc) | SUCCESS);
}