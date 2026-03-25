/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   indexing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	sort_array(int *arr, int size)
{
	int	i;
	int	j;
	int	tmp;

	i = 0;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (arr[i] > arr[j])
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static int	binary_search(int *arr, int value, int size)
{
	int	lo;
	int	hi;
	int	mid;

	lo = 0;
	hi = size - 1;
	while (lo <= hi)
	{
		mid = (lo + hi) / 2;
		if (arr[mid] == value)
			return (mid);
		else if (arr[mid] < value)
			lo = mid + 1;
		else
			hi = mid - 1;
	}
	return (-1);
}

int	normalize_data(t_deque *a)
{
	int		*arr;
	t_node	*curr;
	int		i;

	arr = malloc(sizeof(int) * a->size);
	if (!arr)
		return (0);
	curr = a->front;
	i = 0;
	while (curr)
	{
		arr[i++] = curr->data;
		curr = curr->next;
	}
	sort_array(arr, a->size);
	curr = a->front;
	while (curr)
	{
		curr->data = binary_search(arr, curr->data, a->size);
		curr = curr->next;
	}
	free(arr);
	return (1);
}
