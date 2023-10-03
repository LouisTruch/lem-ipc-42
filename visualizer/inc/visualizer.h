#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <sys/msg.h>
#include <signal.h>

#include <errno.h>

#include "../../libft/libft.h"

#define IPC_ERROR -1
#define ERRNO_DEFAULT 0
#define TIME_BEFORE_FORCE_EXIT 3

#define MSQ_SPECTATE_KEY "./src"
#define SEM_KEY "./ipc_clear.sh"
#define SHM_KEY "./Makefile"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define BOARD_SIZE 100
#define SPAWNING_POS 65535
#define DEATH_POS 65534

#define FREE_TILE ' '

typedef struct s_msg_spect
{
    long mtype;
    u_int16_t pos[2];
    u_int16_t old_pos[2];
    u_int8_t team;
} t_msg_spect;

typedef enum e_rect
{
    CLEAR,
    PLAYER,
} t_rect;

typedef struct s_SDL
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect rect[2];
    bool active;
} t_SDL;

typedef struct s_game
{
    bool started;
    char board[BOARD_SIZE * BOARD_SIZE];
} t_game;

typedef struct s_ipc
{
    int msqid;
    t_game *game;
} t_ipc;

typedef enum e_sem_operation
{
    UNLOCK,
    LOCK,
} t_sem_operation;

#endif
