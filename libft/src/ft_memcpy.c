/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/08 15:05:22 by bcozic            #+#    #+#             */
/*   Updated: 2018/05/12 11:09:16 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	while ((i * 8) < n - 4)
	{
		*((int64_t *)dst + i) = *((const int64_t *)src + i);
		i++;
	}
	i *= 8;
	if (i + 4 < n - 4)
	{
		*((int *)dst + i) = *((const int *)src + i);
		i += 4;
	}
	while (i < n)
	{
		*((char *)dst + i) = *((const char *)src + i);
		i++;
	}
	return (dst);
}
