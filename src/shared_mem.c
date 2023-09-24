#include "../inc/lemipc.h"

static int get_shmem_block(const key_t key, const size_t size, bool *first_player)
{
    errno = 0;
    int shmid;
    if ((shmid = shmget(key, size, 0)) == IPC_ERROR)
    {
        if (errno == ENOENT)
        {
            *first_player = true;
            return (shmget(key, size, IPC_CREAT | IPC_EXCL | 0666));
        }
        perror("shmget");
        return (SHMGET_ERROR);
    }
    *first_player = false;
    return (shmid);
}

static void *attach_shmem(const int shmid)
{
    errno = 0;
    return (shmat(shmid, NULL, 0));
}

int init_shmem(const char *filepath, t_ipc *ipc)
{
    key_t key;
    if ((key = get_key_t(filepath)) == IPC_ERROR)
    {
        perror("ftok shm");
        return (FTOK_ERROR);
    }

    // Can change key to nothing here ???? idk
    if ((ipc->shm = get_shmem_block(key, get_max_size(), &ipc->first_player)) == IPC_ERROR)
    {
        perror("shmget");
        return (SHMGET_ERROR);
    }

    if ((ipc->game = attach_shmem(ipc->shm)) == (void *)IPC_ERROR)
    {
        perror("shmat");
        return (SHMAT_ERROR);
    }
    return (0);
}

int destroy_shmem(const int shmid)
{
    errno = 0;
    return (shmctl(shmid, IPC_RMID, NULL));
}

int get_shmem_stat(const int shmid, struct shmid_ds *shmid_ds)
{
    errno = 0;
    return (shmctl(shmid, IPC_STAT, shmid_ds));
}