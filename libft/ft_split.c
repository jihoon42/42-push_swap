/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 09:48:33 by jkim2             #+#    #+#             */
/*   Updated: 2025/07/09 23:43:31 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_word_cnt(char const *str, char c)
{
	size_t	count;
	int		chker;

	count = 0;
	chker = 1;
	while (*str && *str == c)
		str++;
	while (*str)
	{
		if (chker && *str != c)
		{
			count++;
			chker = 0;
		}
		if (*str == c)
			chker = 1;
		str++;
	}
	return (count);
}

static void	*cleanup_split(char **arr, size_t n_allocated)
{
	size_t	i;

	i = 0;
	while (i < n_allocated)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (NULL);
}

static char	**set_worddup(char const *s, char c, char **arr)
{
	size_t	word_len;
	size_t	i;

	i = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (!*s)
			break ;
		word_len = 0;
		while (s[word_len] && s[word_len] != c)
			word_len++;
		arr[i] = malloc(word_len + 1);
		if (!arr[i])
			return (cleanup_split(arr, i));
		ft_strlcpy(arr[i], s, word_len + 1);
		s += word_len;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	size_t	word_count;
	char	**result;

	if (!s)
		return (NULL);
	word_count = get_word_cnt(s, c);
	result = malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	return (set_worddup(s, c, result));
}
