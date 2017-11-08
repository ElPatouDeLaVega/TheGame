
SRCS = srcs/main.c\
	srcs/gmath.c\
	srcs/game.c

OBJS = $(SRCS:.c=.o)

CC = gcc

CFLAGS = -lX11 -lm -I./include

#CFLAGS += -Werror -Wextra -Wall

#CFLAGS += -g -fsanitize=address

NAME = game

all : $(NAME)

$(NAME) : $(OBJS)
	gcc $(OBJS) $(CFLAGS) -o $(NAME) 

clean :
	rm -f $(OBJS)

fclean : clean
	rm -f $(NAME)

re : fclean all
