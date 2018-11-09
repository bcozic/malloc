/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 01:09:53 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/09 11:38:28 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"
#include <stdlib.h>

void	check_address(void *to_check, t_zone *zone, char *str, size_t size)
{
	check_page_size("check_address");
	t_mem	*page;

	page = zone->page;
	while (page)
	{
		if (page->size % data->pages_size)
			ft_printf("bad page size %d %d    %s\n", page->size % data->pages_size, page->size, str);
		if ((char*)to_check >= (char*)page->ptr && (char*)to_check + size <= (char*)page->ptr + page->size)
			return ;
		page = page->next;
	}
	page = zone->page;
	while (page)
	{
		ft_printf("%p >= %p && %p < %p\n", to_check, page->ptr, (char*)to_check + size, (char*)page->ptr + page->size);
		if ((char*)to_check >= (char*)page->ptr && (char*)to_check <= (char*)page->ptr + page->size && (char*)to_check + size > (char*)page->ptr + page->size)
		{
			ft_printf("size to big\n");
		}
		
		page = page->next;
	}
	ft_printf("%s size = %d\n", str, size);
	exit(0);
}

static void	check_page(t_mem *page, char *str)
{
	while (page)
	{
		if (page->size % data->pages_size)
		{
			ft_printf("ERROR %s page size %d mod %d\n", str, page->size, page->size % data->pages_size);
			exit(0);
		}
		page = page->next;
	}
}

void	check_page_size(char *str)
{
	check_page(data->data_page.page, str);
	check_page(data->tiny.page, str);
	check_page(data->small.page, str);
	check_page(data->large.page, str);
}
