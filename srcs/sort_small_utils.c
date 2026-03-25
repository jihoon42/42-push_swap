/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_small_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/25 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	same_state(t_small_state *x, t_small_state *y)
{
	int	i;

	if (x->a_size != y->a_size || x->b_size != y->b_size)
		return (0);
	i = 0;
	while (i < x->a_size)
	{
		if (x->a[i] != y->a[i])
			return (0);
		i++;
	}
	i = 0;
	while (i < x->b_size)
	{
		if (x->b[i] != y->b[i])
			return (0);
		i++;
	}
	return (1);
}

int	find_small_state(t_small_state *states, int count, t_small_state *target)
{
	int	idx;

	idx = 0;
	while (idx < count)
	{
		if (same_state(&states[idx], target))
			return (idx);
		idx++;
	}
	return (-1);
}

static void	add_small_state(t_small_solver *solver, t_small_state *next,
		int *tail)
{
	int	idx;

	idx = find_small_state(solver->states, *tail, next);
	if (idx < 0)
	{
		solver->states[*tail] = *next;
		(*tail)++;
	}
}

static void	visit_small_ops(t_small_solver *solver, t_small_state *next,
		int head, int *tail)
{
	int	op;

	op = 0;
	while (op < 11)
	{
		apply_small_op(next, &solver->states[head], op);
		if (!same_state(next, &solver->states[head]))
		{
			next->prev = head;
			next->op = op;
			add_small_state(solver, next, tail);
		}
		op++;
	}
}

int	solve_small_state(t_small_solver *solver, int size)
{
	t_small_state	next;
	int				head;
	int				tail;

	head = 0;
	tail = 1;
	while (head < tail)
	{
		if (is_goal_small_state(&solver->states[head], size))
			return (head);
		visit_small_ops(solver, &next, head, &tail);
		head++;
	}
	return (0);
}
