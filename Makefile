# **************************************************************************** #
#                            PUSH_SWAP Makefile                                #
# **************************************************************************** #

NAME		:= push_swap
BONUS_NAME	:= checker
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror

LIBFT_DIR	:= libft
LIBFT		:= $(LIBFT_DIR)/libft.a
INC			:= -Iincludes -I$(LIBFT_DIR)

SRCS_DIR	:= srcs
SRCS		:= $(SRCS_DIR)/main.c \
			   $(SRCS_DIR)/deque.c \
			   $(SRCS_DIR)/operations.c \
			   $(SRCS_DIR)/operations_dual.c \
			   $(SRCS_DIR)/parsing.c \
			   $(SRCS_DIR)/parsing_utils.c \
			   $(SRCS_DIR)/indexing.c \
			   $(SRCS_DIR)/utils.c \
			   $(SRCS_DIR)/sort_small.c \
			   $(SRCS_DIR)/sort_small_ops.c \
			   $(SRCS_DIR)/sort_small_utils.c \
			   $(SRCS_DIR)/partition.c \
			   $(SRCS_DIR)/greedy.c \
			   $(SRCS_DIR)/greedy_exec.c \
			   $(SRCS_DIR)/greedy_utils.c \
			   $(SRCS_DIR)/last_sort.c

OBJS		:= $(SRCS:.c=.o)

BNS_DIR		:= srcs_bonus
BNS_SRCS	:= $(BNS_DIR)/checker_bonus.c \
			   $(BNS_DIR)/deque_bonus.c \
			   $(BNS_DIR)/operations_bonus.c \
			   $(BNS_DIR)/parsing_bonus.c \
			   $(BNS_DIR)/parsing_utils_bonus.c \
			   $(BNS_DIR)/utils_bonus.c \
			   $(BNS_DIR)/get_next_line_bonus.c \
			   $(BNS_DIR)/get_next_line_utils_bonus.c

BNS_OBJS	:= $(BNS_SRCS:.c=.o)

all: $(NAME)

bonus: $(BONUS_NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(BONUS_NAME): $(LIBFT) $(BNS_OBJS)
	$(CC) $(CFLAGS) $(BNS_OBJS) $(LIBFT) -o $(BONUS_NAME)

$(SRCS_DIR)/%.o: $(SRCS_DIR)/%.c includes/push_swap.h
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(BNS_DIR)/%.o: $(BNS_DIR)/%.c includes/checker_bonus.h
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS) $(BNS_OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all

.PHONY: all clean fclean re bonus
