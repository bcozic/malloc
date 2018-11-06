/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_alloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 04:11:20 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/06 08:48:14 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ft_libc.h"

t_mem	*insert_new_packet(t_mem **list_ptr, t_mem *new_ptr)
{
	t_mem	*current_ptr;
	t_mem	*to_remove;

	current_ptr = *list_ptr;
	if (!current_ptr)
	{
		current_ptr = new_ptr;
		return (new_ptr);
	}
	if (current_ptr->ptr > new_ptr->ptr)
	{
		if ((char*)new_ptr->ptr + new_ptr->size == current_ptr->ptr)
		{
			new_ptr->next = current_ptr->next;
			new_ptr->size += current_ptr->size;
			*list_ptr = new_ptr;
			remove_struct_packet(current_ptr);
			return(new_ptr);
		}
		else
		{
			new_ptr->next = current_ptr;
			list_ptr = &new_ptr;
			return(new_ptr);
		}
	}
	else if (!current_ptr->next)
	{
		if ((char*)current_ptr->ptr + current_ptr->size == new_ptr->ptr)
		{
			current_ptr->size += new_ptr->size;
			remove_struct_packet(new_ptr);
			return(current_ptr);
		}
		else
		{
			current_ptr->next = new_ptr;
			return(new_ptr);
		}
	}
	while (current_ptr->next->next && current_ptr->next->ptr < new_ptr->ptr)
		current_ptr = current_ptr->next;
	if ((char*)current_ptr->ptr + current_ptr->size == new_ptr->ptr)
	{
		current_ptr->size += new_ptr->size;
		remove_struct_packet(new_ptr);
		return(current_ptr);
	}
	if ((char*)new_ptr->ptr + new_ptr->size == current_ptr->next->ptr)
	{
		to_remove = current_ptr->next;
		current_ptr->next = new_ptr;
		new_ptr->size += to_remove->size;
		new_ptr->next = to_remove->next;
		remove_struct_packet(to_remove);
		return(new_ptr);
	}
	else
	{
		if (current_ptr->next)
			new_ptr->next = current_ptr->next->next;
		current_ptr->next = new_ptr;
		return(new_ptr);
	}
}

t_mem		*get_new_page(t_zone *zone, size_t nb_pages)
{
	t_mem	*new_page;
	t_mem	*new_packet;

	new_page = (t_mem*)get_new_struct(sizeof(t_mem));
	new_page->ptr = mmap(NULL, data->pages_size * nb_pages, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	new_page->size = data->pages_size * nb_pages;
	new_page->next = NULL;
	new_packet = (t_mem*)get_new_struct(sizeof(t_mem));
	new_packet->ptr = new_page->ptr;
	new_packet->size = data->pages_size;
	new_packet->next = NULL;
	new_packet = insert_new_packet(&zone->packet, new_packet);
	return (new_packet);
}

static void		div_packet(t_mem *packet, size_t size)
{
	t_mem	*new_packet;

	new_packet = (t_mem*)get_new_struct(sizeof(t_mem));
	new_packet->next = packet->next;
	packet->next = new_packet;
	new_packet->ptr = (char*)packet->ptr + size;
	new_packet->size = packet->size - size;
	packet->size = size;
}

static void		add_used_packet(t_zone *zone, t_mem *packet)
{
	t_mem	*used_packet;
	used_packet = zone->used_packet;
	packet->next = NULL;
	if (used_packet == NULL)
	{
		zone->used_packet = packet;
		return ;
	}
	while (used_packet->next)
		used_packet = used_packet->next;
	used_packet->next = packet;
}

static void		*get_mem(t_zone *zone, t_mem *previous, t_mem *packet, size_t size)
{
	if (packet->size > size)
		div_packet(packet, size);
	if (previous == NULL)
		zone->packet = packet->next;
	else
		previous->next = packet->next;
	add_used_packet(zone, packet);
	return (packet->ptr);
}

void	*get_tiny_small(size_t size, t_zone *zone)
{
	t_mem	*previous_packet;
	t_mem	*current_packet;

	size = ((size / zone->max_size) + (size % zone->max_size ? 1 : 0)) * zone->max_size;
	if (zone->page == NULL)
		zone->page = get_new_page(zone, 1);
	current_packet = zone->packet;
	previous_packet = NULL;
	while (!current_packet || current_packet->size < size)
	{
		previous_packet = current_packet;
		if (current_packet == NULL)
			current_packet = get_new_page(zone, 1);
		else
			current_packet = current_packet->next;
	}
	return (get_mem(zone, previous_packet, current_packet, size));
}
