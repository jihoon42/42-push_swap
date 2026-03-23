/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 04:12:01 by jkim2             #+#    #+#             */
/*   Updated: 2025/07/09 23:43:31 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*ptr_dest;
	unsigned char	trans_c;
	size_t			i;

	ptr_dest = s;
	trans_c = c;
	i = 0;
	while (i < n)
	{
		*ptr_dest = trans_c;
		ptr_dest++;
		i++;
	}
	return (s);
}
