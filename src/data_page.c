/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_page.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 07:55:03 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/14 19:15:41 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"

void			remove_struct_packet(t_mem *packet)
{
	packet->next = g_data->data_page.packet;
	packet->size = sizeof(t_mem);
	packet->ptr = packet;
	g_data->data_page.packet = packet;
}

static t_mem	*get_new_data_page(void)
{
	void	*map;
	t_mem	*new_page;
	t_mem	*new_packet;

	if (g_data->allocate_size + g_data->pages_size
			> g_data->rlimit_memlock.rlim_cur)
		return (0);
	map = mmap(NULL, g_data->pages_size * 3, PROT_READ | PROT_WRITE,
			MAP_ANON | MAP_PRIVATE, -1, 0);
	g_data->allocate_size += g_data->pages_size * 3;
	new_page = (t_mem*)map;
	new_page->ptr = map;
	new_page->size = g_data->pages_size * 3;
	new_page = insert_new_packet(&(&g_data->data_page)->page, new_page);
	new_packet = (t_mem*)(void*)((char*)map + sizeof(t_mem));
	new_packet->ptr = (char*)map + sizeof(t_mem) + sizeof(t_mem);
	new_packet->size = (g_data->pages_size * 3) - sizeof(t_mem) - sizeof(t_mem);
	new_packet->next = NULL;
	new_packet = insert_new_packet(&(&g_data->data_page)->packet, new_packet);
	return (new_packet);
}

void			*get_new_struct(size_t size)
{
	t_mem	*data_packet;
	void	*ret;

	if (g_data->data_page.packet && g_data->data_page.packet->size < size)
		g_data->data_page.packet = g_data->data_page.packet->next;
	if (!g_data->data_page.packet)
		if (!(g_data->data_page.packet = get_new_data_page()))
			return (0);
	data_packet = g_data->data_page.packet;
	ret = data_packet->ptr;
	if (!(data_packet->size - size))
	{
		g_data->data_page.packet = data_packet->next;
	}
	else
	{
		data_packet->ptr = (char*)data_packet->ptr + size;
		data_packet->size -= size;
	}
	return (ret);
}
