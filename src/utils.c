#include "../inc/lemipc.h"

void print_board(const char *board)
{
    int i, j;
    ft_printf("---------------\n");
    for (i = 0; i < BOARD_SIZE; i++)
    {
        for (j = 0; j < BOARD_SIZE; j++)
        {
            ft_printf("%c", *(board + i * BOARD_SIZE + j));
        }
        ft_printf("\n");
    }
}

size_t get_max_size(void)
{
    return (sizeof(t_game));
}

key_t get_key_t(const char *filepath)
{
    errno = 0;
    return (ftok(filepath, 1));
}

int clean_up_ipcs(t_ipc *ipc)
{
    struct shmid_ds shmid_ds;

    sem_lock(ipc->semid[GAME_MUTEX], LOCK);
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
    if (destroy_shmem(ipc->shmid) == IPC_ERROR)
    {
        perror("shmctl");
        return SHMCTL_RMID_ERROR;
    }
    if (destroy_semaphore(ipc->semid[GAME_MUTEX]) == IPC_ERROR)
    {
        perror("semctl rmid");
        return SEMCTL_RM_ERROR;
    }
    if (destroy_semaphore(ipc->semid[WAITING_START_MUTEX]) == IPC_ERROR)
    {
        perror("semctl rmid");
        return SEMCTL_RM_ERROR;
    }
    if (destroy_msq(ipc->msqid) == IPC_ERROR)
    {
        perror("msgctl rmid");
        return MSGCTL_RMID_ERROR;
    }
    return SUCCESS;
}