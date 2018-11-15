/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_alloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 04:11:20 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/14 18:58:14 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"

t_mem	*get_new_page(t_zone *zone, size_t nb_pages)
{
	t_mem	*new_page;
	t_mem	*new_packet;

	if (!(new_page = (t_mem*)get_new_struct(sizeof(t_mem))))
		return (0);
	if (g_data->allocate_size + (g_data->pages_size * nb_pages)
			> g_data->rlimit_memlock.rlim_cur)
		return (0);
	new_page->ptr = mmap(NULL, g_data->pages_size * nb_pages,
			PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	g_data->allocate_size += g_data->pages_size * nb_pages;
	new_page->size = g_data->pages_size * nb_pages;
	new_page->next = NULL;
	if (!(new_packet = (t_mem*)get_new_struct(sizeof(t_mem))))
		return (0);
	new_packet->ptr = new_page->ptr;
	new_page = insert_new_packet(&zone->page, new_page);
	new_packet->size = g_data->pages_size * nb_pages;
	new_packet->next = NULL;
	new_packet = insert_new_packet(&zone->packet, new_packet);
	return (new_packet);
}

void	*get_mem(t_zone *zone, t_mem *previous, t_mem *packet, size_t size)
{
	if (packet->size > size)
		if (!(div_packet(packet, size)))
			return (0);
	if (previous == NULL)
		zone->packet = packet->next;
	else
		previous->next = packet->next;
	add_used_packet(zone, packet);
	return (packet->ptr);
}

void	*get_alloc(size_t size, t_zone *zone, size_t nb_page)
{
	t_mem	*previous_packet;
	t_mem	*current_packet;

	size = ((size / zone->size_packet_zone)
			+ (size % zone->size_packet_zone ? 1 : 0))
			* zone->size_packet_zone;
	if (!nb_page)
		nb_page = size / g_data->pages_size;
	if (zone->page == NULL)
		if (!get_new_page(zone, nb_page))
			return (0);
	current_packet = zone->packet;
	previous_packet = NULL;
	while (!current_packet || current_packet->size < size)
	{
		previous_packet = current_packet;
		if (current_packet == NULL)
		{
			if (!(current_packet = get_new_page(zone, nb_page)))
				return (0);
		}
		else
			current_packet = current_packet->next;
	}
	return (get_mem(zone, previous_packet, current_packet, size));
}
