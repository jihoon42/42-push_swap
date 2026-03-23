/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   greedy_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	exec_rr(t_deque *a, t_deque *b, int ra, int rb)
{
	while (ra > 0 && rb > 0)
	{
		rotate_all(a, b);
		ra--;
		rb--;
	}
	while (ra-- > 0)
		rotate(a);
	while (rb-- > 0)
		rotate(b);
}

static void	exec_rrr(t_deque *a, t_deque *b, int rra, int rrb)
{
	while (rra > 0 && rrb > 0)
	{
		reverse_all(a, b);
		rra--;
		rrb--;
	}
	while (rra-- > 0)
		reverse_rotate(a);
	while (rrb-- > 0)
		reverse_rotate(b);
}

static void	exec_ra_rrb(t_deque *a, t_deque *b, int ra, int rrb)
{
	while (ra-- > 0)
		rotate(a);
	while (rrb-- > 0)
		reverse_rotate(b);
}

static int	get_best_case(int ra, int rra, int rb, int rrb)
{
	int	costs[4];
	int	best;
	int	i;

	costs[0] = ra;
	if (rb > ra)
		costs[0] = rb;
	costs[1] = rra;
	if (rrb > rra)
		costs[1] = rrb;
	costs[2] = ra + rrb;
	costs[3] = rra + rb;
	best = 0;
	i = 1;
	while (i < 4)
	{
		if (costs[i] < costs[best])
			best = i;
		i++;
	}
	return (best);
}

void	execute_moves(t_deque *a, t_deque *b, int ba, int bb)
{
	int	best;

	best = get_best_case(ba, a->size - ba, bb, b->size - bb);
	if (best == 0)
		exec_rr(a, b, ba, bb);
	else if (best == 1)
		exec_rrr(a, b, a->size - ba, b->size - bb);
	else if (best == 2)
		exec_ra_rrb(a, b, ba, b->size - bb);
	else
	{
		while (ba < a->size)
		{
			reverse_rotate(a);
			ba++;
		}
		while (bb-- > 0)
			rotate(b);
	}
	push(a, b);
}
