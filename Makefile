NAME = lemipc

SRC	= src/main.c

OBJ	= ${SRC:.c=.o}

LIB = libft/libft.a

CC = clang

CFLAGS = -Wall -Wextra -Werror -g 

RM = rm -f

all : ${NAME}

.c.o : ${CC} ${CFLAGS} -c $ -o ${<:.c=.o}

${NAME} : ${OBJS_MANDATORY}
	make -C libft
	${CC} ${CFLAGS} -o ${NAME_MANDATORY} ${OBJS_MANDATORY} ${LIB}


clean :
	make -C libft clean
	${RM} ${OBJS_MANDATORY} ${OBJS_BONUS}

fclean :
	make -C libft fclean
	${RM} ${NAME_MANDATORY} ${OBJS_MANDATORY} ${NAME_BONUS} ${OBJS_BONUS}

re : fclean all

.PHONY	: all clean fclean re
