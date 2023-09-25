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

    ft_printf("Cleaning up...\n");
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