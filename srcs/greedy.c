/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   greedy.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	max_val(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

static int	calc_min_cost(int ra, int rra, int rb, int rrb)
{
	int	cost;
	int	tmp;

	cost = max_val(ra, rb);
	tmp = max_val(rra, rrb);
	if (tmp < cost)
		cost = tmp;
	tmp = ra + rrb;
	if (tmp < cost)
		cost = tmp;
	tmp = rra + rb;
	if (tmp < cost)
		cost = tmp;
	return (cost);
}

static void	find_best(t_deque *a, t_deque *b, int *best_a, int *best_b)
{
	t_node	*curr;
	int		b_pos;
	int		a_pos;
	int		cost;
	int		min_cost;

	curr = b->front;
	b_pos = 0;
	min_cost = INT_MAX;
	while (curr)
	{
		a_pos = get_target_position(a, curr->data);
		cost = calc_min_cost(a_pos, a->size - a_pos,
				b_pos, b->size - b_pos);
		if (cost < min_cost)
		{
			min_cost = cost;
			*best_a = a_pos;
			*best_b = b_pos;
		}
		curr = curr->next;
		b_pos++;
	}
}

void	greedy(t_deque *a, t_deque *b)
{
	int	best_a;
	int	best_b;

	while (b->size)
	{
		find_best(a, b, &best_a, &best_b);
		execute_moves(a, b, best_a, best_b);
	}
	last_sort(a);
}
