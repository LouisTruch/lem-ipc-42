#include "../inc/lemipc.h"

int sem_lock(int semid, int operation)
{
    struct sembuf sem_op;
    sem_op.sem_op = (operation == UNLOCK) ? 1 : -1;
    sem_op.sem_num = 0;
    sem_op.sem_flg = 0;
    if (semop(semid, &sem_op, 1) == IPC_ERROR)
        return SEMOP_ERROR;
    return 0;
}

static int init_sem(int *sem, const char *filepath, int init_value, bool *first_player)
{
    key_t key;
    if ((key = get_key_t(filepath)) == IPC_ERROR)
    {
        perror("ftok sem");
        return FTOK_ERROR;
    }

    errno = ERRNO_DEFAULT;
    union semun
    {
        int val;
        struct semid_ds *buf;
        ushort array[1];
    } sem_attr;
    // struct semid_ds semid_ds;
    *sem = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL);
    if (*sem != IPC_ERROR)
    {
        *first_player = true;
        sem_attr.val = init_value;
        if (semctl(*sem, 0, SETVAL, sem_attr) == IPC_ERROR)
        {
            perror("semctl setval");
            return SEMCTL_SETVAL_ERROR;
        }
    }
    else if (errno == EEXIST)
    {
        *first_player = false;
        *sem = semget(key, 1, 0);
        if (*sem == IPC_ERROR)
        {
            perror("semget");
            return SEMGET_ERROR;
        }
        // ???
        // while (semid_ds.sem_otime == 0)
        // {
        //     usleep(100);
        // }
    }
    else
    {
        perror("semget");
        return SEMGET_ERROR;
    }
    return SUCCESS;
}

int init_sems(int *sem, bool *first_player)
{
    int err;
    if ((err = init_sem(&sem[WAITING_START_MUTEX], SEM_WAITING_GAME_KEY, 0, first_player)))
        return err;
    if ((err = init_sem(&sem[GAME_MUTEX], SEM_GAME_MUTEX_KEY, 1, first_player)))
        return err;
    return SUCCESS;
}

int destroy_semaphore(int semid)
{
    return (semctl(semid, 0, IPC_RMID));
}