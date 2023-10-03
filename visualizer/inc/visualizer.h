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

#define MSQ_SPECTATE_KEY "./src"
#define SEM_KEY "./ipc_clear.sh"
#define SHM_KEY "./Makefile"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define BOARD_SIZE 50

#define FREE_TILE ' '

typedef struct s_msg_spect
{
    long mtype;
    char mtext[BOARD_SIZE * BOARD_SIZE];
} t_msg_spect;

typedef struct s_SDL
{
    SDL_Window *window;
    SDL_Renderer *renderer;
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
