/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 05:56:46 by jkim2             #+#    #+#             */
/*   Updated: 2025/07/09 23:43:31 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	while (src[i])
	{
		if (i + 1 < size)
			dst[i] = src[i];
		i++;
	}
	if (size > 0)
	{
		if (i < size)
			dst[i] = '\0';
		else
			dst[size - 1] = '\0';
	}
	return (i);
}
