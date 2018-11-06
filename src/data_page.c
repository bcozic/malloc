/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_page.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 07:55:03 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/06 11:33:56 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"

void			remove_struct_packet(t_mem *packet)
{
	t_mem	*current_ptr;
	t_mem	*to_remove;

	// to remove
	check_address(packet, &data->data_page, "Error remove_struct_packet packet", sizeof(t_mem));
	//end remove

	ft_bzero(packet, sizeof(t_mem));
	current_ptr = data->data_page.packet;
	if (!current_ptr)
		current_ptr = packet;
	while (current_ptr->next && current_ptr->next < packet)
		current_ptr = current_ptr->next;
	if (current_ptr + current_ptr->size == packet)
	{
		current_ptr->size += packet->size;
		// to remove
		check_address(current_ptr->ptr, &data->data_page, "Error remove_struct_packet current_ptr->ptr", current_ptr->size);
		//end remove
	}
	else if (packet + packet->size == current_ptr->next)
	{
		to_remove = current_ptr->next;
		current_ptr->next = packet;
		packet->size += to_remove->size;
		packet->next = to_remove->next;
		// to remove
		check_address(packet->ptr, &data->data_page, "Error remove_struct_packet current_ptr->ptr", packet->size);
		//end remove
		remove_struct_packet(to_remove);
	}
	else
	{
		if (current_ptr->next)
			packet->next = current_ptr->next->next;
		current_ptr->next = packet;
	}
}

static t_mem	*get_new_data_page()
{
	void	*map;
	t_mem	*new_page;
	t_mem	*new_packet;
	
	map = mmap(NULL, data->pages_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	new_page = (t_mem*)map;
	new_page->ptr = map;
	new_page->size = data->pages_size;
	new_page->next = NULL;
	new_packet = insert_new_packet(&(&data->data_page)->page, new_page);
	new_packet = (t_mem*)(void*)((char*)map + sizeof(t_mem));
	new_packet->ptr = (char*)map + sizeof(t_mem) + sizeof(t_mem*);
	new_packet->size = data->pages_size;
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
	t_mem	*data_packet;
	t_mem	*previous;
	void	*ret;

	data_packet = data->data_page.packet;
	if (!data_packet)
		data->data_page.packet = get_new_data_page();
	previous = NULL;
	while (data_packet && data_packet->size < size)
	{
		previous = data_packet;
		data_packet = data_packet->next;
	}
	if (!data_packet)
		data_packet = get_new_data_page();
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
		check_address(ret, &data->data_page, "Error get_new_struct ret", size);
		check_address(data_packet->ptr, &data->data_page, "Error get_new_struct data_packet->ptr", data_packet->size);
		// end delete
	}
	return (ret);
}
