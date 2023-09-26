#include "../inc/lemipc.h"

static int get_shmem_block(const key_t key, const size_t size)
{
    errno = ERRNO_DEFAULT;
    int shmid;
    if ((shmid = shmget(key, size, 0)) == IPC_ERROR)
    {
        if (errno == ENOENT)
            return (shmget(key, size, IPC_CREAT | IPC_EXCL | 0666));
        perror("shmget");
        return SHMGET_ERROR;
    }
    return shmid;
}

static void *attach_shmem(const int shmid)
{
    errno = ERRNO_DEFAULT;
    return (shmat(shmid, NULL, 0));
}

int init_shmem(t_ipc *ipc)
{
    key_t key;
    if ((key = get_key_t(SHM_KEY)) == IPC_ERROR)
    {
        perror("ftok shm");
        return FTOK_ERROR;
    }

    // Can change key to nothing here ???? idk
    if ((ipc->shmid = get_shmem_block(key, get_max_size())) == IPC_ERROR)
    {
        perror("shmget");
        return SHMGET_ERROR;
    }

    if ((ipc->game = attach_shmem(ipc->shmid)) == (void *)IPC_ERROR)
    {
        perror("shmat");
        return SHMAT_ERROR;
    }
    return SUCCESS;
}

int destroy_shmem(const int shmid)
{
    errno = ERRNO_DEFAULT;
    return (shmctl(shmid, IPC_RMID, NULL));
}

int get_shmem_stat(const int shmid, struct shmid_ds *shmid_ds)
{
    errno = ERRNO_DEFAULT;
    return (shmctl(shmid, IPC_STAT, shmid_ds));
}