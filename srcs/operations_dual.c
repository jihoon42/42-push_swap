/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_dual.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	do_swap(t_deque *q)
{
	t_node	*first;
	t_node	*second;

	if (q->size < 2)
		return ;
	first = pop_front(q);
	second = pop_front(q);
	push_front(q, first);
	push_front(q, second);
}

void	swap_all(t_deque *a, t_deque *b)
{
	do_swap(a);
	do_swap(b);
	write(1, "ss\n", 3);
}

void	rotate_all(t_deque *a, t_deque *b)
{
	t_node	*node;

	if (a->size >= 2)
	{
		node = pop_front(a);
		push_back(a, node);
	}
	if (b->size >= 2)
	{
		node = pop_front(b);
		push_back(b, node);
	}
	write(1, "rr\n", 3);
}

void	reverse_all(t_deque *a, t_deque *b)
{
	t_node	*node;

	if (a->size >= 2)
	{
		node = pop_back(a);
		push_front(a, node);
	}
	if (b->size >= 2)
	{
		node = pop_back(b);
		push_front(b, node);
	}
	write(1, "rrr\n", 4);
}
