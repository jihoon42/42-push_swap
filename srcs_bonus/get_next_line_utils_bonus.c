/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42gyeongsan.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 02:46:12 by jkim2             #+#    #+#             */
/*   Updated: 2026/03/23 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

size_t	newline_index(t_stash *stash)
{
	size_t	i;

	if (!stash->content || stash->size == 0)
		return (0);
	i = 0;
	while (i < stash->size)
	{
		if (stash->content[i] == '\n')
			return (i);
		i++;
	}
	return (stash->size);
}

void	stash_clear(t_stash *stash)
{
	free(stash->content);
	stash->content = NULL;
	stash->size = 0;
	stash->cap = 0;
}

int	stash_reserve(t_stash *stash, size_t add)
{
	char	*new_buf;
	size_t	new_cap;
	size_t	i;

	if (stash->size + add <= stash->cap)
		return (0);
	new_cap = stash->cap;
	if (new_cap == 0)
		new_cap = 1;
	while (new_cap < stash->size + add)
		new_cap *= 2;
	new_buf = malloc(new_cap);
	if (!new_buf)
		return (stash_clear(stash), 1);
	i = 0;
	while (i < stash->size)
	{
		new_buf[i] = stash->content[i];
		i++;
	}
	free(stash->content);
	stash->content = new_buf;
	stash->cap = new_cap;
	return (0);
}
