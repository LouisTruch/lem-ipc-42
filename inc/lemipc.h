#ifndef LEMPIC_H
#define LEMPIC_H

#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
// #include <semaphore.h>
// #include <fcntl.h> // sem_open() O_CREAT
#include <sys/sem.h>

#include "../libft/libft.h"

#define IPC_ERROR (-1)

#define FTOK_SHM_FILEPATH (argv[0])
#define FTOK_SEMAPHORE_PATH "./mySem"
#define MAX_PLAYER (64)
#define BOARD_SIZE (10)

typedef struct s_player
{
    size_t x;
    size_t y;
    size_t team;
} t_player;

typedef struct s_game
{
    // char board[BOARD_SIZE][BOARD_SIZE];
    char board[BOARD_SIZE * BOARD_SIZE];
    size_t nb_player;
    t_player player[MAX_PLAYER];
} t_game;

typedef struct s_shm
{
    int id;
    key_t key;
} t_shm;

typedef struct s_sem
{
    int id;
    key_t key;
} t_sem;

typedef struct s_ipc
{
    t_game *game;
    bool first_player;
    t_shm shm;
    t_sem sem;
} t_ipc;

typedef enum s_error
{
    FTOK_ERROR = 1,
    SHMGET_ERROR,
    SHMAT_ERROR,
    SHMCTL_STAT_ERROR,
    SHMCTL_RMID_ERROR,
    SEMGET_ERROR,
    SEMCTL_RM_ERROR,
    SEMCTL_SETVAL_ERROR,
    SEMOP_ERROR,
    SEMCTL_STAT_ERROR
} t_error;

typedef enum s_sem_operation
{
    INCREMENT,
    DECREMENT,
} t_sem_operation;

// Shared memory
int init_shmem(const char *filepath, t_ipc *shm_struct);
int get_shmem_stat(const int shmid, struct shmid_ds *shmid_ds);
int destroy_shmem(const int shmid);

// Semaphore
int get_semaphore(t_ipc *ipc);
int sem_operation(int semid, int operation);
int destroy_semaphore(int semid);

// Utils
key_t get_key_t(const char *filepath);
size_t get_max_size(void);
void print_board(const char *board);

#endif