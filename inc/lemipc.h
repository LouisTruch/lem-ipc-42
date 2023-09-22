#ifndef LEMPIC_H
#define LEMPIC_H

#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>   // time()
#include <unistd.h> // Sleep()

#include "../libft/libft.h"

#define IPC_ERROR -1
#define SUCCESS 0

#define FTOK_SHM_FILEPATH argv[0]
#define FTOK_SEMAPHORE_PATH "./mySem"

#define GAME_STARTED 1
#define SECOND_BEFORE_START 2
#define MAX_PLAYER 64
#define MIN_PLAYER 4
#define BOARD_SIZE 15
#define NB_TEAM 2
#define FREE_TILE 'x'

typedef struct s_player
{
    size_t x;
    size_t y;
    size_t team;
} t_player;

typedef struct s_game
{
    bool started;
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

typedef enum s_sem_name
{
    WAITING_START,
    GAME_OPERATION,
} t_sem_name;

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
int sem_operation(int semid, int sem_num, int operation);
int destroy_semaphore(int semid);

// Player
int init_game(t_ipc *ipc);
void set_player_spawn(t_game *game);
void set_players_team(t_ipc *ipc);
int add_player(t_ipc *ipc);

// Utils
key_t get_key_t(const char *filepath);
size_t get_max_size(void);
void print_board(const char *board);

#endif