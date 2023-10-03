#include "../inc/visualizer.h"

void init_SDL(t_SDL *SDL)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        ft_putstr_fd("SDL_Init: error\n", 2);
        exit(EXIT_FAILURE);
    }

    SDL->window = SDL_CreateWindow("Lemipc-visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!SDL->window)
    {
        ft_putstr_fd("SDL_CreateWindow: error\n", 2);
        exit(EXIT_FAILURE);
    }

    SDL->renderer = SDL_CreateRenderer(SDL->window, -1, SDL_RENDERER_ACCELERATED);
    if (!SDL->renderer)
    {
        ft_putstr_fd("SDL_CreateRenderer: error\n", 2);
        exit(EXIT_FAILURE);
    }

    SDL->active = true;
}

void init_ipc(t_ipc *ipc)
{
    errno = ERRNO_DEFAULT;
    key_t key;
    if ((key = ftok(MSQ_SPECTATE_KEY, 1)) == IPC_ERROR)
    {
        perror("ftok msq");
        exit(EXIT_FAILURE);
    }

    ipc->msqid = msgget(key, 0);
    if (ipc->msqid == IPC_ERROR)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }
    return;
}

void set_player_color(t_SDL *SDL, const char c)
{
    switch (c)
    {
    case '1':
        SDL_SetRenderDrawColor(SDL->renderer, 0, 0, 255, 0);
        break;
    case '2':
        SDL_SetRenderDrawColor(SDL->renderer, 255, 0, 0, 0);
        break;
    case '3':
        SDL_SetRenderDrawColor(SDL->renderer, 0, 255, 0, 0);
        break;
    case '4':
        SDL_SetRenderDrawColor(SDL->renderer, 255, 255, 0, 0);
        break;
    case '5':
        SDL_SetRenderDrawColor(SDL->renderer, 255, 0, 255, 0);
        break;
    case '6':
        SDL_SetRenderDrawColor(SDL->renderer, 255, 128, 128, 0);
        break;
    case '7':
        SDL_SetRenderDrawColor(SDL->renderer, 128, 128, 128, 0);
        break;
    case '8':
        SDL_SetRenderDrawColor(SDL->renderer, 128, 0, 0, 0);
        break;
    case '9':
        SDL_SetRenderDrawColor(SDL->renderer, 255, 128, 0, 0);
        break;
    }
}

void draw_board(t_SDL *SDL, const char *board)
{
    SDL_Rect clear;
    clear.x = 0;
    clear.y = 0;
    clear.w = WINDOW_WIDTH;
    clear.h = WINDOW_HEIGHT;
    SDL_SetRenderDrawColor(SDL->renderer, 0, 0, 0, 255);
    SDL_RenderClear(SDL->renderer);

    SDL_Rect rect;
    rect.w = WINDOW_HEIGHT / BOARD_SIZE;
    rect.h = WINDOW_WIDTH / BOARD_SIZE;
    for (int line = 0; line < BOARD_SIZE; line++)
    {
        for (int row = 0; row < BOARD_SIZE; row++)
        {
            if (board[line * BOARD_SIZE + row] != FREE_TILE)
            {
                rect.x = line * rect.w;
                rect.y = row * rect.h;
                set_player_color(SDL, board[line * BOARD_SIZE + row]);
                SDL_RenderFillRect(SDL->renderer, &rect);
            }
        }
    }
    SDL_RenderPresent(SDL->renderer);
}

void alarm_handler(int sigint)
{
    (void)sigint;
}

bool rcv_msg(t_msg_spect *msg, t_ipc *ipc)
{
    signal(SIGALRM, alarm_handler);
    alarm(3);
    if (msgrcv(ipc->msqid, msg, (sizeof(t_msg_spect) - sizeof(long)), 0, 0) == IPC_ERROR)
    {
        return false;
    }
    alarm(0);
    return true;
}

void check_input(t_SDL *SDL)
{
    SDL_Event e;
    while (SDL_PollEvent(&e) > 0)
    {
        switch (e.type)
        {
        case SDL_KEYDOWN:
            if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                SDL->active = false;
                break;
            }
            printf("%i\n", e.key.keysym.scancode);
            break;
        case SDL_QUIT:
            SDL->active = false;
            break;
        }
    }
}

void start_loop(t_SDL *SDL, t_ipc *ipc)
{
    t_msg_spect msg;
    while (SDL->active)
    {
        check_input(SDL);
        if (rcv_msg(&msg, ipc))
            draw_board(SDL, msg.mtext);
        else
            break;
    }
}

int main(void)
{
    t_SDL SDL;
    init_SDL(&SDL);

    t_ipc ipc;
    init_ipc(&ipc);

    start_loop(&SDL, &ipc);

    if (msgctl(ipc.msqid, IPC_RMID, NULL) == IPC_ERROR)
    {
        perror("msgctl RMID");
        return EXIT_FAILURE;
    }
    SDL_DestroyWindow(SDL.window);
    SDL_Quit();

    return EXIT_SUCCESS;
}