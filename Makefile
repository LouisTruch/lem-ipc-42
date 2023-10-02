NAME = lemipc
NAME_VISU = lemipc-visualizer

SRC	= $(wildcard src/*.c)
SRC_VISU = $(wildcard visualizer/src/*.c)

OBJ	= ${SRC:.c=.o}
OBJ_VISU = ${SRC_VISU:.c=.o}

LIB = libft/libft.a

CC = clang

CFLAGS = -Wall -Wextra -Werror -g
CFLAGS_VISU = -Wall -Wextra -Werror -g `sdl2-config --cflags`
LDFLAGS= `sdl2-config --libs`

RM = rm -f

all : ${NAME} ${NAME_VISU}

%.o : %.c
	${CC} ${CFLAGS_VISU} -c $< -o $@

visu: ${NAME_VISU}

${NAME} : ${OBJ}
	make -C libft
	${CC} -o ${NAME} ${OBJ} ${LIB}

${NAME_VISU} : ${OBJ_VISU}
	make -C libft
	${CC} -o ${NAME_VISU} ${OBJ_VISU} ${LIB} ${LDFLAGS}

clean :
	make -C libft clean
	${RM} ${OBJ} ${OBJ_VISU}

fclean : clean
	make -C libft fclean
	${RM} ${NAME} ${NAME_VISU}

r : ${NAME} ${NAME_VISU}
	clear
	bash run.sh

re : fclean all

.PHONY	: all clean fclean re
