/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	init_deque(t_deque *q, char name)
{
	q->name = name;
	q->size = 0;
	q->front = NULL;
	q->back = NULL;
}

static void	sort_stack(t_deque *a, t_deque *b)
{
	if (a->size == 2)
		sort_two(a);
	else if (a->size == 3)
		sort_three(a);
	else
	{
		partition_to_b(a, b);
		greedy(a, b);
	}
}

int	main(int argc, char **argv)
{
	t_deque	a;
	t_deque	b;

	if (argc < 2)
		return (0);
	init_deque(&a, 'a');
	init_deque(&b, 'b');
	if (!parse_args(argc, argv, &a))
	{
		free_deque(&a);
		write(2, "Error\n", 6);
		return (1);
	}
	if (!is_sorted(&a))
	{
		normalize_data(&a);
		sort_stack(&a, &b);
	}
	free_deque(&a);
	free_deque(&b);
	return (0);
}
