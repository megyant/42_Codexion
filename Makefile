# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/25 15:50:00 by mbotelho          #+#    #+#              #
#    Updated: 2026/04/30 09:33:31 by mbotelho         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion
CC = cc
FLAGS = -Wall -Werror -Wextra -g

FOLDER = coders

SRCS = $(FOLDER)/main.c \
	   $(FOLDER)/parsing.c \
	   $(FOLDER)/utils.c \
	   $(FOLDER)/init.c \
	   $(FOLDER)/free.c \
	   $(FOLDER)/coder.c \
	   $(FOLDER)/mutex_thread_safe.c \
	   $(FOLDER)/time.c \
	   $(FOLDER)/monitor.c \
	   $(FOLDER)/get_set.c \
	   $(FOLDER)/simulation.c \
	   $(FOLDER)/sim_utils.c \
	   $(FOLDER)/dongles.c \
	   $(FOLDER)/heap.c

OBJS = $(SRCS:.c=.o)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

all_clean: all clean

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re all_clean