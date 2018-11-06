/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 01:09:53 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/06 07:44:08 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"

void	check_address(void *to_check, t_zone *zone, char *str, size_t size)
{
	t_mem	*page;

	page = zone->page;
	while (page)
	{
		if ((char*)to_check >= (char*)page->ptr && (char*)to_check + size <= (char*)page->ptr + page->size)
			return ;
		page = page->next;
	}
	page = zone->page;
	while (page)
	{
		ft_printf("%p >= %p && %p < %p\n", to_check, page->ptr, (char*)to_check + size, (char*)page->ptr + page->size);
		page = page->next;	
	}
	ft_printf("%s\n", str);
}
