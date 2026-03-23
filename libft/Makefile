# **************************************************************************** #
#                               LIBFT Makefile                                 #
# **************************************************************************** #

NAME    := libft.a
CC      := cc
CFLAGS  := -Wall -Wextra -Werror

SRCS := \
ft_atoi.c ft_bzero.c ft_calloc.c ft_isdigit.c ft_memset.c \
ft_putstr_fd.c ft_split.c ft_strlcpy.c ft_strlen.c ft_strncmp.c

OBJS := $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c libft.h
	$(CC) $(CFLAGS) -I. -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
