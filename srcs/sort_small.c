/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_small.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	init_small_solver(t_small_solver *solver, t_deque *a)
{
	t_node	*curr;
	int		i;

	ft_bzero(solver->states[0].a, sizeof(solver->states[0].a));
	ft_bzero(solver->states[0].b, sizeof(solver->states[0].b));
	solver->states[0].a_size = a->size;
	solver->states[0].b_size = 0;
	solver->states[0].prev = -1;
	solver->states[0].op = -1;
	curr = a->front;
	i = 0;
	while (curr)
	{
		solver->states[0].a[i++] = curr->data;
		curr = curr->next;
	}
}

int	is_goal_small_state(t_small_state *state, int size)
{
	int	i;

	if (state->a_size != size || state->b_size != 0)
		return (0);
	i = 0;
	while (i < size)
	{
		if (state->a[i] != i)
			return (0);
		i++;
	}
	return (1);
}

void	sort_five(t_deque *a, t_deque *b)
{
	t_small_solver	solver;
	int				goal;
	int				len;

	init_small_solver(&solver, a);
	goal = solve_small_state(&solver, a->size);
	len = 0;
	while (solver.states[goal].prev != -1)
	{
		solver.path[len++] = solver.states[goal].op;
		goal = solver.states[goal].prev;
	}
	while (len-- > 0)
		replay_small_op(a, b, solver.path[len]);
}

void	sort_two(t_deque *a)
{
	if (a->front->data > a->front->next->data)
		swap(a);
}

void	sort_three(t_deque *a)
{
	int	first;
	int	second;
	int	third;

	first = a->front->data;
	second = a->front->next->data;
	third = a->back->data;
	if (first > second && second < third && first < third)
		swap(a);
	else if (first > second && second > third)
	{
		swap(a);
		reverse_rotate(a);
	}
	else if (first > second && second < third && first > third)
		rotate(a);
	else if (first < second && second > third && first < third)
	{
		swap(a);
		rotate(a);
	}
	else if (first < second && second > third && first > third)
		reverse_rotate(a);
}
