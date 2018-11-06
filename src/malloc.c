/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/02 22:35:05 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/06 05:01:09 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"

t_data				*data = NULL;

static void	init_data(void)
{
	size_t		pages_size;
	void		*map;
	void		*tmp;

	pages_size = (size_t)getpagesize();
	if (pages_size <= 0)
		return ;
	map = mmap(NULL, pages_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	data = (t_data*)map;
	ft_bzero(map, pages_size);
	data->pages_size = pages_size;
	data->tiny.max_size = TINY_PK;
	data->small.max_size = SMALL_PK;
	tmp = ((char*)map + sizeof(t_data));
	data->data_page.page = (t_mem*)tmp;
	data->data_page.page->ptr = map;
	data->data_page.page->size = pages_size;
	tmp = ((char*)map + sizeof(t_data) + sizeof(t_mem));
	data->data_page.packet = (t_mem*)tmp;
	data->data_page.packet->ptr = data->data_page.packet + sizeof(t_mem);
	data->data_page.packet->size = (size_t)((char*)map - (char*)data->data_page.packet->ptr) + pages_size;
	data->data_page.packet->next = NULL;

	//to delete
	check_address(data, &data->data_page, "Error init data", sizeof(t_data));
	check_address(data->data_page.page, &data->data_page, "Error init data->data_page.page", sizeof(t_mem));
	check_address(data->data_page.packet, &data->data_page, "Error init data->data_page.packet", sizeof(t_mem));
	check_address(data->data_page.packet->ptr, &data->data_page, "Error init data->data_page.packet->ptr", data->data_page.packet->size);
	//end delete
}

void		*ft_malloc(size_t size)
{
	if (data == NULL)
		init_data();
	if (data->pages_size <= 0)
		return (0);
	if (size <= TINY_ZONE)
		return (get_tiny_small(size, &data->tiny));
	else if (size <= SMALL_ZONE)
		return (get_tiny_small(size, &data->small));
	// else
	// 	return (get_large(size));
	return (0);
}

	// tab = (char*)mmap(NULL, (size_t)data.pages_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	// munmap(tab, (size_t)data.pages_size);
	// return (data.pages_size);
