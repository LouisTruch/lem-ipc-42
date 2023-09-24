#include "../inc/lemipc.h"

void print_board(const char *board)
{
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++)
    {
        for (j = 0; j < BOARD_SIZE; j++)
        {
            ft_printf("%c", *(board + i * BOARD_SIZE + j));
        }
        ft_printf("\n");
    }
}

size_t get_max_size(void)
{
    return (sizeof(t_game));
}

key_t get_key_t(const char *filepath)
{
    errno = 0;
    return (ftok(filepath, 1));
}