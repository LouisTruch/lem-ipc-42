#!/usr/bin/bash

if (killall lemipc); then
    echo "Killing lemipc proccess"
fi

shmid=$(ipcs -m | grep 666 | grep ltruchel | awk '{print $2}' | head -n 1)
if [ -n "$shmid" ]; then
    echo "Deleting a Shared Memory Block"
    ipcrm -m ${shmid}
else
    echo "No Shared Memory to delete"
fi

semid=$(ipcs -s | grep 666 | awk '{print $2}' | head -n 1)
if [ -n "$semid" ]; then
    echo "Deleting a Semaphore"
    ipcrm -s ${semid}
else
    echo "No Semaphore to delete"
fi

semid=$(ipcs -s | grep 666 | awk '{print $2}' | head -n 1)
if [ -n "$semid" ]; then
    echo "Deleting a Semaphore"
    ipcrm -s ${semid}
else
    echo "No Semaphore to delete"
fi

msqid=$(ipcs -q | grep 666 | awk '{print $2}' | head -n 1)
if [ -n "$msqid" ]; then
    echo "Deleting a Message Queue"
    ipcrm -q ${msqid}
else
    echo "No Message Queue to delete"
fi