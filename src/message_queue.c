#include "../inc/lemipc.h"

void recv_msq(int msq, t_msg *msg, int team)
{
    (void)team;
    if (msgrcv(msq, &msg, sizeof(msg), 1, IPC_NOWAIT) == IPC_ERROR)
    {
        perror("msgrcv");
        return;
    }
}

void send_msq(int msq, t_msg msg)
{
    if (msgsnd(msq, &msg, sizeof(msg), 0) == IPC_ERROR)
    {
        perror("msgsnd");
    }
}

int init_msq(int *msqid)
{
    key_t key;
    if ((key = get_key_t(MSQ_KEY)) == IPC_ERROR)
    {
        perror("ftok msq");
        return (FTOK_ERROR);
    }

    errno = 0;
    *msqid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL);
    if (errno == EEXIST)
    {
        *msqid = msgget(key, 0);
        if (*msqid == IPC_ERROR)
        {
            perror("msgget");
            return MSGGET_ERROR;
        }
    }
    else if (*msqid == IPC_ERROR)
    {
        perror("msgget");
        return MSGGET_ERROR;
    }
    return SUCCESS;
}

int destroy_msq(int msqid)
{
    return (msgctl(msqid, IPC_RMID, NULL));
}