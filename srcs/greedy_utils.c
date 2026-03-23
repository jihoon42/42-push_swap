/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   greedy_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	get_min_position(t_deque *a)
{
	t_node	*curr;
	int		min_val;
	int		min_pos;
	int		pos;

	curr = a->front;
	min_val = curr->data;
	min_pos = 0;
	pos = 0;
	while (curr)
	{
		if (curr->data < min_val)
		{
			min_val = curr->data;
			min_pos = pos;
		}
		curr = curr->next;
		pos++;
	}
	return (min_pos);
}

int	get_target_position(t_deque *a, int b_data)
{
	t_node	*curr;
	int		target_pos;
	int		closest;
	int		pos;

	curr = a->front;
	target_pos = 0;
	closest = INT_MAX;
	pos = 0;
	while (curr)
	{
		if (curr->data > b_data && curr->data < closest)
		{
			closest = curr->data;
			target_pos = pos;
		}
		curr = curr->next;
		pos++;
	}
	if (closest == INT_MAX)
		target_pos = get_min_position(a);
	return (target_pos);
}
