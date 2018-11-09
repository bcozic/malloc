/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_alloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 04:11:20 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/09 16:09:03 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ft_libc.h"
#include <stdlib.h>//must be remove

static int	join_ptr(t_mem *mem1, t_mem *mem2)
{
	if ((char*)mem1->ptr + mem1->size == (char*)mem2)
	{
		mem1->next = mem2->next;
		mem1->size += mem2->size;
		remove_struct_packet(mem2);
		return(1);
	}
	return(0);
}

t_mem	*insert_new_packet(t_mem **list_ptr, t_mem *new_ptr)
{
	check_page_size("insert_new_packet");
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

t_mem		*get_new_page(t_zone *zone, size_t nb_pages)
{
	check_page_size("get_new_page");
	t_mem	*new_page;
	t_mem	*new_packet;

	if (!(new_page = (t_mem*)get_new_struct(sizeof(t_mem))))
	{
		ft_printf("get_new_page1\n");
		return (0);
	}
	if (data->allocate_size + (data->pages_size * nb_pages) > data->rlimit_memlock.rlim_cur)
	{
		ft_printf("get_new_page2\n");
		return (0);
	}
	new_page->ptr = mmap(NULL, data->pages_size * nb_pages, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	//ft_printf("mmap %d pages\n", nb_pages);
	
	data->allocate_size += data->pages_size * nb_pages;
	new_page->size = data->pages_size * nb_pages;
	new_page->next = NULL;

	if (!(new_packet = (t_mem*)get_new_struct(sizeof(t_mem))))
	{
		ft_printf("get_new_page3\n");
		return(0);
	}
	new_packet->ptr = new_page->ptr;
	new_page->next = zone->page;
	zone->page = new_page;
	//new_page = insert_new_packet(&zone->page, new_page);
	new_packet->size = data->pages_size * nb_pages;
	new_packet->next = NULL;
	new_packet = insert_new_packet(&zone->packet, new_packet);

//
	check_address(new_packet->ptr, zone, "Error get_new_page new_packet->ptr", new_packet->size);
	check_address(new_page->ptr, zone, "Error get_new_page new_packet->ptr", new_page->size);
//	
	return (new_packet);
}

static int		div_packet(t_mem *packet, size_t size, t_zone *zone)//delete zone
{
	check_page_size("div_packet");
	(void)zone;
	t_mem	*new_packet;

	if (!(new_packet = (t_mem*)get_new_struct(sizeof(t_mem))))
		return (0);
	new_packet->next = packet->next;
	packet->next = new_packet;
	new_packet->ptr = (char*)packet->ptr + size;
	new_packet->size = packet->size - size;
	packet->size = size;

	//
	check_address(new_packet->ptr, zone, "Error div_packet new_packet->ptr", new_packet->size);
	check_address(new_packet, &data->data_page, "Error div_packet new_packet", sizeof(t_mem));
	//
	return(1);
}

static void		add_used_packet(t_zone *zone, t_mem *packet)
{
	check_page_size("add_used_packet");
	packet->next = NULL;
	if (zone->used_packet)
		packet->next = zone->used_packet->next;
	zone->used_packet = packet;
}

static void		*get_mem(t_zone *zone, t_mem *previous, t_mem *packet, size_t size)
{
	check_page_size("get_mem");
	if (packet->size > size)
		if (!(div_packet(packet, size, zone)))
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
	check_page_size("get_alloc");
	t_mem	*previous_packet;
	t_mem	*current_packet;

	size = ((size / zone->size_packet_zone) + (size % zone->size_packet_zone ? 1 : 0)) * zone->size_packet_zone;
	if (!nb_page)
		nb_page = size / data->pages_size;
	if (zone->page == NULL)
		if (!get_new_page(zone, nb_page))
		{
			ft_printf("get alloc 1\n");
			return(0);
		}
	current_packet = zone->packet;
	previous_packet = NULL;
	while (!current_packet || current_packet->size < size)
	{
		previous_packet = current_packet;
		if (current_packet == NULL)
		{
			if (!(current_packet = get_new_page(zone, nb_page)))
			{
				ft_printf("get alloc 2\n");
				return(0);
			}
		}
		else
			current_packet = current_packet->next;
	}
	return (get_mem(zone, previous_packet, current_packet, size));
}
