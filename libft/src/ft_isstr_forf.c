/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isstr_forf.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/03 11:56:21 by bcozic            #+#    #+#             */
/*   Updated: 2018/02/03 13:38:17 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_isstr_forf(char *str, int (*f)(int), int (*f2)(int))
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (!f((int)str[i]) && !(f2((int)str[i])))
			return (0);
		i++;
	}
	return (1);
}