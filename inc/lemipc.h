#ifndef LEMPIC_H
#define LEMPIC_H

#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>   // time()
#include <unistd.h> // Sleep()
#include <sys/ioctl.h>
#include <fcntl.h> // open()...

#include "../libft/libft.h"

#define SUCCESS 0
#define IPC_ERROR -1
#define ERRNO_DEFAULT 0
#define ASCII_OFFSET 48

#define SHM_KEY "./lemipc"
#define SEM_GAME_MUTEX_KEY "./mySem"
#define SEM_WAITING_GAME_KEY "./mySem1"
#define SEM_SPECTATE_MUTEX_KEY "./ipc_clear.sh"
#define MSQ_KEY "Makefile"
#define URANDOM_PATH "/dev/urandom"

#define TIME_BEFORE_START 2
#define NB_OPPONENT_TO_DIE 2
#define MAX_PLAYER 64
#define MIN_PLAYER 4
#define BOARD_SIZE 10
#define NB_TEAM 5
#define FREE_TILE ' '
#define GAME_SPEED 500000
#define NO_PATH -1

#define CURRENT_TILE (p_coord[LINE] * BOARD_SIZE + p_coord[ROW])
#define TOP_TILE ((p_coord[LINE] - 1) * BOARD_SIZE + p_coord[ROW])
#define BOTTOM_TILE ((p_coord[LINE] + 1) * BOARD_SIZE + p_coord[ROW])
#define LEFT_TILE (p_coord[LINE] * BOARD_SIZE + (p_coord[ROW] - 1))
#define RIGHT_TILE (p_coord[LINE] * BOARD_SIZE + (p_coord[ROW] + 1))

#define RESET "\033[0m"
#define WHITE "\033[37m"
#define BOLDRED "\033[1m\033[31m"
#define BOLDGREEN "\033[1m\033[32m"
#define BOLDYELLOW "\033[1m\033[33m"
#define BOLDBLUE "\033[1m\033[34m"
#define BOLDMAGENTA "\033[1m\033[35m"
#define BOLDCYAN "\033[1m\033[36m"
#define BOLDWHITE "\033[1m\033[37m"

typedef enum e_coord
{
    LINE,
    ROW,
    TEAM,
} t_coord;

typedef struct s_player
{
    int coord[2];
    long team;
} t_player;

typedef struct s_game
{
    bool started;
    char board[BOARD_SIZE * BOARD_SIZE];
} t_game;

typedef struct s_ipc
{
    t_game *game;
    bool first_player;
    int shmid;
    int semid[3];
    int msqid;
    t_player player[1];
} t_ipc;

typedef struct s_msg
{
    long mtype;
    int mtext[3];
} t_msg;

typedef enum e_error
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
    SEMCTL_STAT_ERROR,
    MSGGET_ERROR,
    MSGCTL_RMID_ERROR,
    GAME_STARTED
} t_error;

typedef enum e_sem_name
{
    WAITING_START_MUTEX,
    GAME_MUTEX,
    SPECTATE_MUTEX,
} t_sem_name;

typedef enum e_sem_operation
{
    UNLOCK,
    LOCK,
} t_sem_operation;

typedef enum e_direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    CANT_MOVE,
} t_direction;

// Shared memory
int init_shmem(t_ipc *ipc);
int get_shmem_stat(const int shmid, struct shmid_ds *shmid_ds);
int destroy_shmem(const int shmid);

// IPCS
void init_ipcs(t_ipc *ipc);

// Semaphore
int init_sems(int *sem, bool *first_player);
int sem_lock(int semid, int operation);
int destroy_semaphore(int semid);

// Message queue
int init_msq(int *msqid);
void send_msq(int msq, t_msg *msg);
void recv_msq(int msq, t_msg *msg, long team);
int destroy_msq(int msqid);

// Player
int init_game(t_ipc *ipc);
void add_player(t_ipc *ipc);
void set_player_spawn(t_ipc *ipc);
void move_player(char *board, int p_coord[2], const int direction);

// Game
void start_game(t_ipc *ipc);
void start_spectating(t_ipc *ipc);

// Utils
key_t get_key_t(const char *filepath);
size_t get_max_size(void);
int clean_up_ipcs(t_ipc *ipc);
bool is_tile_free(const char tile);
void print_board(const char *board);

#endif