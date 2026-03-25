/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	is_valid_number(const char *s)
{
	int	i;

	i = 0;
	if (s[i] == '-' || s[i] == '+')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

long	ft_atol(const char *s)
{
	long			sign;
	unsigned long	nbr;
	unsigned long	limit;
	int				digit;

	nbr = 0;
	sign = 1;
	while (*s == ' ' || (*s >= 9 && *s <= 13))
		s++;
	if (*s == '-' || *s == '+')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	limit = LONG_MAX;
	if (sign < 0)
		limit++;
	while (*s >= '0' && *s <= '9')
	{
		digit = *s - '0';
		if (nbr > (limit - (unsigned long)digit) / 10)
		{
			if (sign < 0)
				return (LONG_MIN);
			return (LONG_MAX);
		}
		nbr = nbr * 10 + (unsigned long)digit;
		s++;
	}
	if (sign < 0)
	{
		if (nbr == (unsigned long)LONG_MAX + 1UL)
			return (LONG_MIN);
		return (-(long)nbr);
	}
	return ((long)nbr);
}

int	has_duplicate(t_deque *a, int value)
{
	t_node	*curr;

	curr = a->front;
	while (curr)
	{
		if (curr->data == value)
			return (1);
		curr = curr->next;
	}
	return (0);
}
