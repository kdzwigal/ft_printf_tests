NAME = print.out

CC = cc

CFLAGS = -Wall -Wextra -Werror -Wno-format-overflow -fsanitize=address -g -I./ft_printf

SRCS = test.c

SRC = main.c

OBJS = $(SRCS:.c=.o)

LIBFT = ./ft_printf/libftprintf.a
HEADER = ./ft_printf/ft_printf.h

all: $(NAME) clean run

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

run: $(NAME)
	./$(NAME)

re: fclean all

rec: fclean all clean

.PHONY: all clean fclean re rec
