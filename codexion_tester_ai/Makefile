CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g
NAME    = tester

all: $(NAME)

$(NAME): tester.c
	$(CC) $(CFLAGS) tester.c -o $(NAME)

clean:
	rm -f $(NAME)
	rm -f helgrind_log_*.txt

.PHONY: all clean
