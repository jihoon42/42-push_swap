/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "../libft/libft.h"
# include <limits.h>

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

typedef struct s_small_state
{
	int	a[5];
	int	b[5];
	int	a_size;
	int	b_size;
	int	prev;
	int	op;
}	t_small_state;

typedef struct s_small_solver
{
	t_small_state	states[720];
	int				path[32];
}	t_small_solver;

/* deque.c */
void	push_front(t_deque *q, t_node *x);
void	push_back(t_deque *q, t_node *x);
t_node	*pop_front(t_deque *q);
t_node	*pop_back(t_deque *q);
t_node	*new_node(int data);

/* operations.c */
void	swap(t_deque *q);
void	push(t_deque *dst, t_deque *src);
void	rotate(t_deque *q);
void	reverse_rotate(t_deque *q);

/* operations_dual.c */
void	swap_all(t_deque *a, t_deque *b);
void	rotate_all(t_deque *a, t_deque *b);
void	reverse_all(t_deque *a, t_deque *b);

/* parsing.c */
int		parse_args(int argc, char **argv, t_deque *a);

/* parsing_utils.c */
int		is_valid_number(const char *s);
long	ft_atol(const char *s);
int		has_duplicate(t_deque *a, int value);

/* indexing.c */
int		normalize_data(t_deque *a);

/* sort_small.c */
void	sort_two(t_deque *a);
void	sort_three(t_deque *a);
void	sort_five(t_deque *a, t_deque *b);
void	apply_small_op(t_small_state *dst, t_small_state *src, int op);
void	replay_small_op(t_deque *a, t_deque *b, int op);
int		find_small_state(t_small_state *states, int count,
			t_small_state *target);
int		is_goal_small_state(t_small_state *state, int size);
int		solve_small_state(t_small_solver *solver, int size);

/* partition.c */
void	partition_to_b(t_deque *a, t_deque *b);

/* greedy.c */
void	greedy(t_deque *a, t_deque *b);

/* greedy_exec.c */
void	execute_moves(t_deque *a, t_deque *b, int best_a, int best_b);

/* greedy_utils.c */
int		get_target_position(t_deque *a, int b_data);
int		get_min_position(t_deque *a);

/* last_sort.c */
void	last_sort(t_deque *a);

/* utils.c */
int		is_sorted(t_deque *a);
void	free_deque(t_deque *q);

#endif
