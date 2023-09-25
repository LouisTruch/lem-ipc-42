#include "../inc/lemipc.h"

int main(int argc, char **argv)
{
    (void)argc;
    int err;
    t_ipc ipc;
    if ((err = init_shmem(FTOK_SHM_FILEPATH, &ipc)))
        return err;
    if ((err = init_msq(&ipc.msq)))
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
    start_game(&ipc);

    return (clean_up_ipcs(&ipc) | SUCCESS);
}