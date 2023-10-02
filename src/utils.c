#include "../inc/lemipc.h"

static char *color(int color)
{
    switch (color)
    {
    case 49:
        return BOLDRED;
    case 50:
        return BOLDGREEN;
    case 51:
        return BOLDYELLOW;
    case 52:
        return BOLDBLUE;
    case 53:
        return BOLDMAGENTA;
    case 54:
        return BOLDCYAN;
    case 55:
        return BOLDWHITE;
    default:
        break;
    }
    return WHITE;
}

void print_board(const char *board)
{
    char top_border[BOARD_SIZE + 2];
    ft_memset(top_border, '-', sizeof(top_border));
    ft_printf("%s\n", top_border);
    // char t = "\U0001F600";
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        ft_printf("|");
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            ft_printf("%s%c%s", color(*(board + i * BOARD_SIZE + j)), *(board + i * BOARD_SIZE + j), RESET);
        }
        ft_printf("|\n");
    }
    ft_printf("%s\n", top_border);
}

key_t get_key_t(const char *filepath)
{
    errno = ERRNO_DEFAULT;
    return (ftok(filepath, 1));
}

bool is_tile_free(const char tile)
{
    if (tile == FREE_TILE)
        return true;
    return false;
}