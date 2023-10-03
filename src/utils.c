#include "../inc/lemipc.h"

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