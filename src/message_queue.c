#include "../inc/lemipc.h"

void send_msq(int msqid, t_msg *msg)
{
    errno = ERRNO_DEFAULT;
    if (msgsnd(msqid, msg, (sizeof(t_msg) - sizeof(long)), 0) == IPC_ERROR)
    {
        perror("msgsnd");
    }
}
void recv_msq(int msqid, t_msg *msg, long team)
{
    errno = ERRNO_DEFAULT;
    if (msgrcv(msqid, msg, (sizeof(t_msg) - sizeof(long)), team, IPC_NOWAIT) == IPC_ERROR)
    {
        if (errno != ENOMSG)
            perror("msgrcv");
        return;
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

    errno = ERRNO_DEFAULT;
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