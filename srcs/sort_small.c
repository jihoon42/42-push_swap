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
