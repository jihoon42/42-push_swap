/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	is_sorted(t_deque *a)
{
	t_node	*curr;

	curr = a->front;
	while (curr && curr->next)
	{
		if (curr->data > curr->next->data)
			return (0);
		curr = curr->next;
	}
	return (1);
}

void	free_deque(t_deque *q)
{
	t_node	*curr;
	t_node	*next;

	if (!q)
		return ;
	curr = q->front;
	while (curr)
	{
		next = curr->next;
		free(curr);
		curr = next;
	}
	q->front = NULL;
	q->back = NULL;
	q->size = 0;
}

void	error_exit(t_deque *a, char **split)
{
	int	i;

	if (split)
	{
		i = 0;
		while (split[i])
			free(split[i++]);
		free(split);
	}
	free_deque(a);
	write(2, "Error\n", 6);
	exit(1);
}
