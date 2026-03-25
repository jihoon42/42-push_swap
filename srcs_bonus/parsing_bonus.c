/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

static int	process_token(t_deque *a, const char *token)
{
	long	value;

	if (!is_valid_number(token))
		return (0);
	value = ft_atol(token);
	if (value > INT_MAX || value < INT_MIN)
		return (0);
	if (has_duplicate(a, (int)value))
		return (0);
	push_back(a, new_node((int)value));
	return (1);
}

static void	free_tokens(char **tokens, int start)
{
	while (tokens[start])
		free(tokens[start++]);
	free(tokens);
}

static int	parse_one_arg(t_deque *a, char *arg)
{
	char	**tokens;
	int		j;

	tokens = ft_split(arg, ' ');
	if (!tokens)
		return (-1);
	if (!tokens[0])
		return (free(tokens), 0);
	j = 0;
	while (tokens[j])
	{
		if (!process_token(a, tokens[j]))
			return (free_tokens(tokens, j), -1);
		free(tokens[j]);
		j++;
	}
	free(tokens);
	return (0);
}

int	parse_args(int argc, char **argv, t_deque *a)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (parse_one_arg(a, argv[i]) < 0)
			return (-1);
		i++;
	}
	return (a->size > 0);
}
