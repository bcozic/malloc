/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/11 16:35:23 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/16 21:38:23 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"

void	*calloc(size_t count, size_t size)
{
	void	*ret;

	ret = malloc(count * size);
	ft_bzero(ret, count * size);
	return (ret);
}
