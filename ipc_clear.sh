#!/usr/bin/bash

shmid=$(ipcs -m | grep 666 | awk '{print $2}' | head -n 1)
if [ -n "$shmid" ]; then
    echo "Deleting a Shared Memory block"
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