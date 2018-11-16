/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fnc_packets.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/14 18:43:54 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/14 19:14:15 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"

static int	join_ptr(t_mem *mem1, t_mem *mem2)
{
	if ((char*)mem1->ptr + mem1->size == (char*)mem2->ptr)
	{
		mem1->next = mem2->next;
		mem1->size += mem2->size;
		remove_struct_packet(mem2);
		return (1);
	}
	return (0);
}

t_mem		*insert_new_packet(t_mem **list_ptr, t_mem *new_ptr)
{
	t_mem	*current_ptr;

	current_ptr = *list_ptr;
	if (!current_ptr || new_ptr->ptr < current_ptr->ptr)
	{
		new_ptr->next = current_ptr;
		if (current_ptr)
			join_ptr(new_ptr, current_ptr);
		*list_ptr = new_ptr;
		return (new_ptr);
	}
	while (current_ptr->next && new_ptr->ptr > current_ptr->next->ptr)
		current_ptr = current_ptr->next;
	new_ptr->next = current_ptr->next;
	current_ptr->next = new_ptr;
	if (join_ptr(current_ptr, new_ptr))
		new_ptr = current_ptr;
	if (current_ptr->next)
		join_ptr(new_ptr, current_ptr->next);
	return (new_ptr);
}

t_mem		*cut_packet(t_mem *packet, void *ptr)
{
	t_mem	*new_packet;

	new_packet = (t_mem*)get_new_struct(sizeof(t_mem));
	new_packet->next = packet->next;
	new_packet->ptr = ptr;
	new_packet->size = ((size_t)packet->ptr + packet->size) - (size_t)ptr;
	packet->next = new_packet;
	packet->size -= new_packet->size;
	return (new_packet);
}

void		add_used_packet(t_zone *zone, t_mem *packet)
{
	packet->next = zone->used_packet;
	zone->used_packet = packet;
}

int			div_packet(t_mem *packet, size_t size)
{
	t_mem	*new_packet;

	if (!(new_packet = (t_mem*)get_new_struct(sizeof(t_mem))))
		return (0);
	new_packet->next = packet->next;
	packet->next = new_packet;
	new_packet->ptr = (char*)packet->ptr + size;
	new_packet->size = packet->size - size;
	packet->size = size;
	return (1);
}
