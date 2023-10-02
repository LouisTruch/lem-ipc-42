NAME = lemipc

SRC	= src/main.c src/shared_mem.c src/semaphore.c src/utils.c src/player.c src/game.c src/message_queue.c src/ipcs.c

OBJ	= ${SRC:.c=.o}

LIB = libft/libft.a

CC = clang

CFLAGS = -Wall -Wextra -g 

RM = rm -f

all : ${NAME}

.c.o : ${CC} ${CFLAGS} -c $ -o ${<:.c=.o}

${NAME} : ${OBJ}
	make -C libft
	${CC} ${CFLAGS} -o ${NAME} ${OBJ} ${LIB}


clean :
	make -C libft clean
	${RM} ${OBJ}

fclean :
	make -C libft fclean
	${RM} ${NAME} ${OBJ}

r : ${NAME}
	clear
	bash run.sh

re : fclean all

.PHONY	: all clean fclean re t
