/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_bonus.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECKER_BONUS_H
# define CHECKER_BONUS_H

# include "../libft/libft.h"
# include <limits.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_node
{
	int				data;
	struct s_node	*next;
	struct s_node	*prev;
}	t_node;

typedef struct s_deque
{
	char	name;
	int		size;
	t_node	*front;
	t_node	*back;
}	t_deque;

typedef struct s_stash
{
	char	*content;
	size_t	size;
	size_t	cap;
}	t_stash;

/* deque_bonus.c */
void	push_front(t_deque *q, t_node *x);
void	push_back(t_deque *q, t_node *x);
t_node	*pop_front(t_deque *q);
t_node	*pop_back(t_deque *q);
t_node	*new_node(int data);

/* operations_bonus.c */
void	swap_silent(t_deque *q);
void	push_silent(t_deque *dst, t_deque *src);
void	rotate_silent(t_deque *q);
void	rev_rotate_silent(t_deque *q);

/* parsing_bonus.c */
int		parse_args(int argc, char **argv, t_deque *a);

/* parsing_utils_bonus.c */
int		is_valid_number(const char *s);
long	ft_atol(const char *s);
int		has_duplicate(t_deque *a, int value);

/* utils_bonus.c */
void	init_deque(t_deque *q, char name);
int		is_sorted(t_deque *a);
void	free_deque(t_deque *q);

/* get_next_line_bonus.c */
char	*get_next_line(int fd);

/* get_next_line_utils_bonus.c */
size_t	newline_index(t_stash *stash);
void	stash_clear(t_stash *stash);
int		stash_reserve(t_stash *stash, size_t add);

#endif
