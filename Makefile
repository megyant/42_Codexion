# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/25 15:50:00 by mbotelho          #+#    #+#              #
#    Updated: 2026/03/27 09:00:50 by mbotelho         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = codexion
CC = cc
FLAGS = -Wall -Werror -Wextra

FOLDER = coders

SRCS = $(FOLDER)/codexion.c \
	   $(FOLDER)/parsing.c \
	   $(FOLDER)/utils.c

OBJS = $(SRCS:.c=.o)

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