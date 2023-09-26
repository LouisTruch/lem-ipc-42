#include "../inc/lemipc.h"

int main(int argc, char **argv)
{
    struct winsize w;
    if (!ioctl(STDOUT_FILENO, TIOCGWINSZ, &w))
    {
        ft_printf("Terminal info col:%i row:%i\n", w.ws_col, w.ws_row);
    }
    if (argc > 2)
    {
        ft_putstr_fd("Format is : ./lemipc [team-number (optional)]", STDERR_FILENO);
        return EXIT_FAILURE;
    }
    if (argc == 1)
    {
        // Spectate + Clear IPC if necessary (not needed in case of spectator ?)
        return SUCCESS;
    }

    t_ipc ipc;
    // Parsing argv to be done: only a positive int >= 0 && <= 9 or nothing
    ipc.player->team = ft_atoi(argv[1]);
    int err;
    if ((err = init_sems(ipc.semid, &ipc.first_player)))
        return err;
    if ((err = init_shmem(&ipc)))
        return err;
    if ((err = init_msq(&ipc.msqid)))
        return err;

    if (ipc.first_player)
    {
        if ((err = init_game(&ipc)))
            return err;
    }
    else if (!ipc.first_player)
    {
        if ((err = add_player(&ipc)))
        {
            if (err == GAME_STARTED)
            {
                // Spectate + Clear IPC if necessary (not needed in case of spectator ?)
            }
            else
                return err;
        }
    }

    start_game(&ipc);

    return (clean_up_ipcs(&ipc) | SUCCESS);
}