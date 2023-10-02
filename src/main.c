#include "../inc/lemipc.h"

int parse_arg(const char *arg)
{
    for (int i = 0; arg[i]; i++)
    {
        if (!ft_isdigit(arg[i]))
            return -1;
    }
    int team_nb = ft_atoi(arg);
    if (team_nb <= 0 || team_nb > NB_TEAM)
    {
        ft_printf("Team number must be between 1 and %i\n", NB_TEAM);
        exit(EXIT_FAILURE);
    }
    return team_nb;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        ft_putstr_fd("Format is : ./lemipc [team-number(1-9)]", STDERR_FILENO);
        return EXIT_FAILURE;
    }
    t_ipc ipc;
    srand(getpid());
    init_ipcs(&ipc);
    ipc.player->team = parse_arg(argv[1]);
    if (ipc.first_player)
        init_game(&ipc);
    else
        add_player(&ipc);
    start_game(&ipc);
    return (clean_up_ipcs(&ipc));
}