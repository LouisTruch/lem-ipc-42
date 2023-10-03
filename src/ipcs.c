#include "../inc/lemipc.h"

void init_ipcs(t_ipc *ipc)
{
    int err;
    if ((err = init_sems(ipc->semid, &ipc->first_player)))
        exit(err);
    if ((err = init_shmem(ipc)))
        exit(err);
    if ((err = init_msq(&ipc->msqid[PLAY], MSQ_KEY)))
    {
        shmdt(ipc->game);
        exit(err);
    }
    if ((err = init_msq(&ipc->msqid[SPECT], MSQ_SPECTATE_KEY)))
    {
        shmdt(ipc->game);
        exit(err);
    }
}

int clean_up_ipcs(t_ipc *ipc)
{
    struct shmid_ds shmid_ds;
    if (get_shmem_stat(ipc->shmid, &shmid_ds) == IPC_ERROR)
    {
        perror("utils.c clean_up_ipcs(): shmctl");
        return (SHMCTL_STAT_ERROR);
    }
    if (shmid_ds.shm_nattch > 1)
    {
        sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
        return 0;
    }
    sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);

    ft_printf("Cleaning up...\n");
    int err = SUCCESS;
    if (destroy_shmem(ipc->shmid) == IPC_ERROR)
    {
        perror("shmctl");
        err = SHMCTL_RMID_ERROR;
    }
    if (destroy_semaphore(ipc->semid[GAME_MUTEX]) == IPC_ERROR)
    {
        perror("semctl rmid");
        err = SEMCTL_RM_ERROR;
    }
    if (destroy_semaphore(ipc->semid[WAITING_START_MUTEX]) == IPC_ERROR)
    {
        perror("semctl rmid");
        err = SEMCTL_RM_ERROR;
    }
    if (destroy_msq(ipc->msqid[PLAY]) == IPC_ERROR)
    {
        perror("msgctl rmid");
        err = MSGCTL_RMID_ERROR;
    }
    return err;
}