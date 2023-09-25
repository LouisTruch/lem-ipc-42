#include "../inc/lemipc.h"

static bool check_death(const t_player *player, char *board)
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
    return nb_opponent & NB_OPPONENT_TO_DIE;
}

static bool check_game_end(const char *board)
{
    (void)board;
    return false;
}

void start_game(t_ipc *ipc)
{
    semaphore_lock(ipc->sem[GAME_MUTEX], LOCK);

    // Check around player if dead or not
    if (check_death(&ipc->game->player[ipc->player_id], ipc->game->board))
    {

        // exit_game() => remove player from board and detach memory + release lock
        semaphore_lock(ipc->sem[GAME_MUTEX], UNLOCK);
        return;
    }
    if (check_game_end(ipc->game->board))
    {
        semaphore_lock(ipc->sem[GAME_MUTEX], UNLOCK);
        return;
    }

    // Check message queue to see "strat"

    // If no strat establish one ?

    // Move player according to strat

    print_board(ipc->game->board);
    semaphore_lock(ipc->sem[GAME_MUTEX], UNLOCK);
}