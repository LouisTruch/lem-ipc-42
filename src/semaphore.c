#include "../inc/lemipc.h"

int sem_operation(int semid, int operation)
{
    struct sembuf sem_op;
    sem_op.sem_op = (operation == UNLOCK) ? 1 : -1;
    ft_printf("Sem %i\n", sem_op.sem_op);
    sem_op.sem_num = 0;
    sem_op.sem_flg = 0;
    if (semop(semid, &sem_op, 1) == IPC_ERROR)
        return SEMOP_ERROR;
    return 0;
}

int get_sem(int *sem, const char *filepath, const int sem_init_value)
{
    key_t key;
    if ((key = get_key_t(filepath)) == IPC_ERROR)
    {
        perror("ftok sem");
        return FTOK_ERROR;
    }

    errno = 0;
    union semun
    {
        int val;
        struct semid_ds *buf;
        ushort array[1];
    } sem_attr;
    *sem = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL);
    if (*sem != IPC_ERROR)
    {
        sem_attr.val = sem_init_value;
        if (semctl(*sem, 0, SETVAL, sem_attr) == IPC_ERROR)
        {
            perror("semctl setval");
            return SEMCTL_SETVAL_ERROR;
        }
    }
    else if (errno == EEXIST)
    {
        *sem = semget(key, 1, 0);
        if (*sem == IPC_ERROR)
        {
            perror("semget");
            return SEMGET_ERROR;
        }
    }
    else
    {
        perror("semget");
        return SEMGET_ERROR;
    }
    return 0;
}

int destroy_semaphore(int semid)
{
    return (semctl(semid, 0, IPC_RMID));
}