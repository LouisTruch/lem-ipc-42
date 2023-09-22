#include "../inc/lemipc.h"

int sem_operation(int semid, int nsem, int operation)
{
    struct sembuf sem_op[1];
    sem_op->sem_op = (operation == INCREMENT) ? 1 : -1;
    // ft_printf("Sem %i\n", sem_op->sem_op);
    sem_op->sem_num = nsem;
    sem_op->sem_flg = 0;
    if (semop(semid, sem_op, 0) == IPC_ERROR)
        return SEMOP_ERROR;
    return 0;
}

int get_semaphore(t_ipc *ipc)
{
    if ((ipc->sem.key = get_key_t(FTOK_SEMAPHORE_PATH)) == IPC_ERROR)
    {
        perror("ftok sem");
        return FTOK_ERROR;
    }

    errno = 0;
    // No need for file and key ??
    if ((ipc->sem.id = semget(ipc->sem.key, 2, 0)) == IPC_ERROR)
    {
        if (errno == ENOENT)
        {
            // Add protection here ?
            ipc->sem.id = semget(ipc->sem.key, 2, IPC_CREAT | IPC_EXCL | 0666);
            union semun
            {
                int val;
                struct semid_ds *buf;
                ushort array[1];
            } sem_attr;
            sem_attr.val = 0;
            if (semctl(ipc->sem.id, WAITING_START, SETVAL, sem_attr) == IPC_ERROR)
            {
                perror("semctl setvalue");
                return SEMCTL_SETVAL_ERROR;
            }
            sem_attr.val = 1;
            if (semctl(ipc->sem.id, GAME_OPERATION, SETVAL, sem_attr) == IPC_ERROR)
            {
                perror("semctl setvalue");
                return SEMCTL_SETVAL_ERROR;
            }
            return 0;
        }
        perror("semget");
        return SEMGET_ERROR;
    }

    // This is not working ? Should make it so other players wait for player 1 to semctl on the semaphore
    // struct semid_ds semid_ds;
    // if (semctl(ipc->sem.id, 0, IPC_STAT) == IPC_ERROR)
    // {
    //     perror("semctl stat");
    //     return SEMCTL_STAT_ERROR;
    // }
    // ft_printf("sem stat %i\n", semid_ds.sem_otime);
    return 0;
}

int destroy_semaphore(int semid)
{
    return (semctl(semid, 0, IPC_RMID));
}