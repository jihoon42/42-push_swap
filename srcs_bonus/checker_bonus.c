/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

static int	exec_dual(t_deque *a, t_deque *b, char *line)
{
	if (ft_strncmp(line, "ss\n", 4) == 0)
	{
		swap_silent(a);
		swap_silent(b);
	}
	else if (ft_strncmp(line, "rr\n", 4) == 0)
	{
		rotate_silent(a);
		rotate_silent(b);
	}
	else if (ft_strncmp(line, "rrr\n", 5) == 0)
	{
		rev_rotate_silent(a);
		rev_rotate_silent(b);
	}
	else
		return (0);
	return (1);
}

static int	exec_op(t_deque *a, t_deque *b, char *line)
{
	if (ft_strncmp(line, "sa\n", 4) == 0)
		swap_silent(a);
	else if (ft_strncmp(line, "sb\n", 4) == 0)
		swap_silent(b);
	else if (ft_strncmp(line, "pa\n", 4) == 0)
		push_silent(a, b);
	else if (ft_strncmp(line, "pb\n", 4) == 0)
		push_silent(b, a);
	else if (ft_strncmp(line, "ra\n", 4) == 0)
		rotate_silent(a);
	else if (ft_strncmp(line, "rb\n", 4) == 0)
		rotate_silent(b);
	else if (ft_strncmp(line, "rra\n", 5) == 0)
		rev_rotate_silent(a);
	else if (ft_strncmp(line, "rrb\n", 5) == 0)
		rev_rotate_silent(b);
	else
		return (exec_dual(a, b, line));
	return (1);
}

static int	read_and_exec(t_deque *a, t_deque *b)
{
	char	*line;
	int		valid;

	valid = 1;
	line = get_next_line(0);
	while (line)
	{
		if (!exec_op(a, b, line))
		{
			free(line);
			return (0);
		}
		free(line);
		line = get_next_line(0);
	}
	return (valid);
}

static int	err_exit(t_deque *a, t_deque *b)
{
	free_deque(a);
	if (b)
		free_deque(b);
	write(2, "Error\n", 6);
	return (1);
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
		return (err_exit(&a, NULL));
	if (!read_and_exec(&a, &b))
		return (err_exit(&a, &b));
	if (is_sorted(&a) && b.size == 0)
		write(1, "OK\n", 3);
	else
		write(1, "KO\n", 3);
	free_deque(&a);
	free_deque(&b);
	return (0);
}
