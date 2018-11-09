/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_page.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 07:55:03 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/09 16:09:09 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"

void			remove_struct_packet(t_mem *packet)
{
	check_page_size("remove_struct_packet");
	packet->next = data->data_page.packet;
	packet->size = sizeof(t_mem);
	packet->ptr = packet;
	data->data_page.packet = packet;
	// to remove
	check_address(packet->ptr, &data->data_page, "Error remove_struct_packet packet", packet->size);
	// end remove
}

static t_mem	*get_new_data_page()
{
	//ft_printf("new page\n");
	check_page_size("get_new_data_page");

	void	*map;
	t_mem	*new_page;
	t_mem	*new_packet;
	
	if (data->allocate_size + data->pages_size > data->rlimit_memlock.rlim_cur)
	{
		//ft_printf("alloc 1 %d %d\n", data->allocate_size + data->pages_size, data->rlimit_memlock.rlim_cur);
		return (0);
	}
	map = mmap(NULL, data->pages_size * 3, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

	//ft_printf("mmap 3 data pages\n");
	data->allocate_size += data->pages_size * 3;
	new_page = (t_mem*)map;
	new_page->ptr = map;
	new_page->size = data->pages_size * 3;
	new_page->next = NULL;
	new_page->next = data->data_page.page;
	data->data_page.page = new_page;
	//new_packet = insert_new_packet(&(&data->data_page)->page, new_page);
	new_packet = (t_mem*)(void*)((char*)map + sizeof(t_mem));
	new_packet->ptr = (char*)map + sizeof(t_mem) + sizeof(t_mem);
	new_packet->size = (data->pages_size * 3) - sizeof(t_mem) - sizeof(t_mem);
	new_packet->next = NULL;
	new_packet = insert_new_packet(&(&data->data_page)->packet, new_packet);
//
	check_address(new_page->ptr, &data->data_page, "Error get_new_data_page new_page->ptr", new_page->size);
	check_address(new_packet->ptr, &data->data_page, "Error get_new_data_page new_packet->ptr", new_packet->size);
//

	return (new_packet);
}

void			*get_new_struct(size_t size)
{
	check_page_size("get_new_struct");
	t_mem	*data_packet;
	t_mem	*previous;
	void	*ret;

	data_packet = data->data_page.packet;
	if (!data_packet)
		if (!(data->data_page.packet = get_new_data_page()))
			return (0);
	previous = NULL;
	while (data_packet && data_packet->size < size)
	{
		previous = data_packet;
		data_packet = data_packet->next;
	}
	if (!data_packet)
		if (!(data_packet = get_new_data_page()))
			return (0);
	ret = data_packet->ptr;
	if (!(data_packet->size - size))
	{
		if (previous)
			previous->next = data_packet->next;
		else
			data->data_page.packet = data_packet->next;
	}
	else
	{
		data_packet->ptr = (char*)data_packet->ptr + size;
		data_packet->size -= size;
		// to delete
		check_address(data_packet->ptr, &data->data_page, "Error get_new_struct data_packet->ptr", data_packet->size);
		// end delete
	}
	return (ret);
}
