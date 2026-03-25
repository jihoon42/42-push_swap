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

static int	find_pos(t_deque *a, int val)
{
	t_node	*curr;
	int		pos;

	curr = a->front;
	pos = 0;
	while (curr)
	{
		if (curr->data == val)
			return (pos);
		curr = curr->next;
		pos++;
	}
	return (0);
}

static void	move_to_top(t_deque *a, int val)
{
	int	pos;

	pos = find_pos(a, val);
	if (pos <= a->size / 2)
	{
		while (a->front->data != val)
			rotate(a);
	}
	else
	{
		while (a->front->data != val)
			reverse_rotate(a);
	}
}

void	sort_five(t_deque *a, t_deque *b)
{
	move_to_top(a, 0);
	push(b, a);
	if (a->size > 3)
	{
		move_to_top(a, 1);
		push(b, a);
	}
	sort_three(a);
	push(a, b);
	push(a, b);
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
