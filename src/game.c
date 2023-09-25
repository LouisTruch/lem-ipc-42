#include "../inc/lemipc.h"

static bool check_death(t_player *player, const char *board)
{
    int x = player->x;
    int y = player->y;
    int nb_opponent = 0;
    for (int i = x - 1; i <= x + 1; i++)
    {
        for (int j = y - 1; j <= y + 1; j++)
        {
            if (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE)
            {
                if (board[i * BOARD_SIZE + j] != FREE_TILE && board[i * BOARD_SIZE + j] != (int)player->team + '0')
                    nb_opponent++;
            }
        }
    }
    if (nb_opponent >= NB_OPPONENT_TO_DIE)
        player->alive = false;
    return (nb_opponent & NB_OPPONENT_TO_DIE);
}

static bool check_game_end(const t_game *game)
{
    int team = -1;
    for (size_t i = 0; i < game->nb_player; i++)
    {
        if (game->player[i].alive)
        {
            if (team != game->player[i].team && team != -1)
                return true;
            team = game->player[i].team;
        }
    }
    return false;
}

static void leave_game(t_ipc *ipc)
{
    // Need to add cleaning for message queues
    int x = ipc->game->player[ipc->player_id].x;
    int y = ipc->game->player[ipc->player_id].y;
    ipc->game->board[x * BOARD_SIZE + y] = FREE_TILE;
    if (shmdt(ipc->game) == IPC_ERROR)
        perror("shmdt");
}

static void choose_target(const char *board, char team_c, int coord[2])
{
    if (coord[0] == -1)
    {
        ft_printf("Choosing new target\n");
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (board[i * BOARD_SIZE + j] != FREE_TILE && board[i * BOARD_SIZE + j] != team_c)
                {
                    coord[0] = i;
                    coord[1] = j;
                }
            }
        }
    }
    else
    {
        // follow target
    }
}

static bool target_range(int x1, int y1, int x2, int y2)
{

}

static void pathfinding(const char *board, int start_x, int start_y, int end_x, int end_y)
{
    char copy[BOARD_SIZE * BOARD_SIZE];
    // Moove that
    for (int i = 0; i < ft_strlen(board); i++)
    {
        copy[i] = board[i];
    }
    do
    {
        /* code */
    } while (!target_range());
    
}

void start_game(t_ipc *ipc)
{
    t_msg msg;
    msg.msg_type = 1; // <== Team nb
    ft_memset(&msg.coord, -1, sizeof(int) * 2);

    semaphore_lock(ipc->sem[GAME_MUTEX], LOCK);
    ft_printf("Starting game...\n");
    // Check around player if dead or not
    // if (check_death(&ipc->game->player[ipc->player_id], ipc->game->board) || check_game_end(ipc->game))
    // {
    //     leave_game(ipc);
    //     semaphore_lock(ipc->sem[GAME_MUTEX], UNLOCK);
    //     exit(0);
    // }

    recv_msq(ipc->msq, &msg, ipc->game->player[ipc->player_id].team);

    choose_target(ipc->game->board, ipc->game->player[ipc->player_id].team + '0', msg.coord);
    ft_printf("%i %i\n", msg.coord[0], msg.coord[1]);
    pathfinding(ipc->game->board, ipc->game->player[ipc->player_id].x, ipc->game->player[ipc->player_id].y, msg.coord[0], msg.coord[1]);
    // move_player();

    send_msq(ipc->msq, msg);
    print_board(ipc->game->board);
    semaphore_lock(ipc->sem[GAME_MUTEX], UNLOCK);
}