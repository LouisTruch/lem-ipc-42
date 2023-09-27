#include "../inc/lemipc.h"

int parse_arg(const char *arg)
{
    for (int i = 0; arg[i]; i++)
    {
        if (!ft_isdigit(arg[i]))
            return -1;
    }
    return (ft_atoi(arg));
}

int main(int argc, char **argv)
{

    if (argc > 2)
    {
        ft_putstr_fd("Format is : ./lemipc [team-number]", STDERR_FILENO);
        return EXIT_FAILURE;
    }
    if (argc == 1)
    {
        struct winsize w;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w))
            return EXIT_FAILURE;
        ft_printf("Terminal info col:%i row:%i\n", w.ws_col, w.ws_row);
        t_ipc ipc;
        int err;
        if ((err = init_sems(ipc.semid, &ipc.first_player)))
            return err;
        if ((err = init_shmem(&ipc)))
            return err;
        if ((err = init_msq(&ipc.msqid)))
            return err;
        start_spectating(&ipc);
        return (clean_up_ipcs(&ipc) | EXIT_SUCCESS);

        // Spectate + Clear IPC if necessary (not needed in case of spectator ?)
    }

    t_ipc ipc;
    ipc.player->team = parse_arg(argv[1]);
    if (ipc.player->team < 0 || ipc.player->team >= NB_TEAM)
    {
        ft_printf("Team number must be between 0 and %i\n", NB_TEAM - 1);
        return EXIT_FAILURE;
    }

    // Parsing argv to be done: only a positive int >= 0 && <= 9 or nothing
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

    return (clean_up_ipcs(&ipc) | EXIT_SUCCESS);
}