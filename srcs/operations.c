/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	swap(t_deque *q)
{
	t_node	*first;
	t_node	*second;

	if (q->size < 2)
		return ;
	first = pop_front(q);
	second = pop_front(q);
	push_front(q, first);
	push_front(q, second);
	if (q->name == 'a')
		write(1, "sa\n", 3);
	else
		write(1, "sb\n", 3);
}

void	push(t_deque *dst, t_deque *src)
{
	t_node	*node;

	if (src->size == 0)
		return ;
	node = pop_front(src);
	push_front(dst, node);
	if (dst->name == 'a')
		write(1, "pa\n", 3);
	else
		write(1, "pb\n", 3);
}

void	rotate(t_deque *q)
{
	t_node	*node;

	if (q->size < 2)
		return ;
	node = pop_front(q);
	push_back(q, node);
	if (q->name == 'a')
		write(1, "ra\n", 3);
	else
		write(1, "rb\n", 3);
}

void	reverse_rotate(t_deque *q)
{
	t_node	*node;

	if (q->size < 2)
		return ;
	node = pop_back(q);
	push_front(q, node);
	if (q->name == 'a')
		write(1, "rra\n", 4);
	else
		write(1, "rrb\n", 4);
}
