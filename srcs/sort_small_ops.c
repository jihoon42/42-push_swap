/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_small_ops.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/25 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	state_swap(int *stack, int size)
{
	int	tmp;

	if (size < 2)
		return ;
	tmp = stack[0];
	stack[0] = stack[1];
	stack[1] = tmp;
}

static void	state_push(int *dst, int *dst_size, int *src, int *src_size)
{
	int	i;

	if (*src_size == 0)
		return ;
	i = *dst_size;
	while (i > 0)
	{
		dst[i] = dst[i - 1];
		i--;
	}
	dst[0] = src[0];
	i = 0;
	while (i + 1 < *src_size)
	{
		src[i] = src[i + 1];
		i++;
	}
	(*dst_size)++;
	(*src_size)--;
}

static void	state_rotate(int *stack, int size, int reverse)
{
	int	tmp;
	int	i;

	if (size < 2)
		return ;
	i = 0;
	if (!reverse)
	{
		tmp = stack[0];
		while (i + 1 < size)
		{
			stack[i] = stack[i + 1];
			i++;
		}
		stack[size - 1] = tmp;
		return ;
	}
	tmp = stack[size - 1];
	i = size - 1;
	while (i > 0)
	{
		stack[i] = stack[i - 1];
		i--;
	}
	stack[0] = tmp;
}

void	apply_small_op(t_small_state *dst, t_small_state *src, int op)
{
	*dst = *src;
	if (op == 0 || op == 2)
		state_swap(dst->a, dst->a_size);
	if (op == 1 || op == 2)
		state_swap(dst->b, dst->b_size);
	if (op == 3)
		state_push(dst->a, &dst->a_size, dst->b, &dst->b_size);
	else if (op == 4)
		state_push(dst->b, &dst->b_size, dst->a, &dst->a_size);
	if (op == 5 || op == 7)
		state_rotate(dst->a, dst->a_size, 0);
	if (op == 6 || op == 7)
		state_rotate(dst->b, dst->b_size, 0);
	if (op == 8 || op == 10)
		state_rotate(dst->a, dst->a_size, 1);
	if (op == 9 || op == 10)
		state_rotate(dst->b, dst->b_size, 1);
}

void	replay_small_op(t_deque *a, t_deque *b, int op)
{
	if (op == 0)
		swap(a);
	else if (op == 1)
		swap(b);
	else if (op == 2)
		swap_all(a, b);
	else if (op == 3)
		push(a, b);
	else if (op == 4)
		push(b, a);
	else if (op == 5)
		rotate(a);
	else if (op == 6)
		rotate(b);
	else if (op == 7)
		rotate_all(a, b);
	else if (op == 8)
		reverse_rotate(a);
	else if (op == 9)
		reverse_rotate(b);
	else
		reverse_all(a, b);
}
