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

// Delete
void talk(t_ipc *ipc)
{
    sem_lock(ipc->semid[WAITING_START_MUTEX], LOCK);
    for (int i = 0; i < 5; i++)
    {
        sem_lock(ipc->semid[GAME_MUTEX], LOCK);
        t_msg msg;
        msg.mtype = 1;
        ft_memset(msg.mtext, -1, sizeof(msg.mtext));
        usleep(10000);
        if (msgrcv(ipc->msqid, &msg, (sizeof(t_msg) - sizeof(long)), 1, IPC_NOWAIT) == ENOMSG)
        {
        }
        ft_printf("rcv %i mypid%i\n", msg.mtext[0], getpid());
        // sem_lock(ipc->semid[GAME_MUTEX], LOCK);
        t_msg send;
        send.mtype = 1;
        send.mtext[0] = getpid();
        msgsnd(ipc->msqid, &send, (sizeof(t_msg) - sizeof(long)), 0);
        // msgsnd(ipc->msqid, &send, (sizeof(t_msg) - sizeof(long)), 0);
        // sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
        sem_lock(ipc->semid[GAME_MUTEX], UNLOCK);
    }
}

void test_init(t_ipc *ipc)
{
    sleep(2);
    sem_lock(ipc->semid[WAITING_START_MUTEX], UNLOCK);
    sem_lock(ipc->semid[WAITING_START_MUTEX], UNLOCK);
    // t_msg send;
    // send.mtype = 1;
    // send.mtext[0] = getpid();
    // msgsnd(ipc->msqid, &send, (sizeof(t_msg) - sizeof(long)), 0);
    // exit(0);
}

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        ft_putstr_fd("Format is : ./lemipc [team-number]", STDERR_FILENO);
        return EXIT_FAILURE;
    }
    t_ipc ipc;
    if (argc == 1)
    {
        sleep(TIME_BEFORE_START);
        init_ipcs(&ipc);
        start_spectating(&ipc);
        return (clean_up_ipcs(&ipc));
    }
    init_ipcs(&ipc);
    ipc.player->team = parse_arg(argv[1]);
    if (ipc.first_player)
        init_game(&ipc);
    else
        add_player(&ipc);
    start_game(&ipc);
    return (clean_up_ipcs(&ipc));
}