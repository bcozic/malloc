/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 02:18:32 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/06 12:32:07 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"

static void	remove_all_page(t_mem *page)
{
	if (!page)
		return ;
	remove_all_page(page->next);
	munmap(page->ptr, page->size);
}

static void	check_fogotten_free()
{
	t_mem	*current;
	size_t	size;

	size = 0;
	current = data->tiny.used_packet;
	while (current)
	{
		size += current->size;
		current = current->next;
	}
	current = data->small.used_packet;
	while (current)
	{
		size += current->size;
		current = current->next;
	}
	current = data->large.used_packet;
	while (current)
	{
		size += current->size;
		current = current->next;
	}
	if (size)
		ft_printf("WARNING you have %d leaks\nI'm kind, I free it for you but go fix it\nFor inable auto-free call auto_free(0)\n", size);
}

static int	check_is_free(t_mem *page, t_mem *packet)
{
	while (packet)
	{
		ft_printf("packet->ptr %p end ptr %p page->ptr %p end ptr %p\n", packet->ptr, (char*)packet->ptr + packet->size, page->ptr, (char*)page->ptr + page->size);
		if ((packet->ptr >= page->ptr && (char*)packet->ptr < (char*)page->ptr + page->size) || ((char*)packet->ptr + packet->size >= (char*)page->ptr && (char*)packet->ptr + packet->size < (char*)page->ptr + page->size))
		{
			ft_printf("leak1\n");
			return (0);
		}
		packet = packet->next;
		ft_printf("packet %p\n", packet);
	}
	return (1);
}

static void	remove_free_page(t_zone *zone, t_mem *page)
{
	if (!page)
		return ;
	// if (page->size > data->pages_size)
	// 	cut_multi_page(page);
	remove_free_page(zone, page->next);
	ft_printf("used %p\n", zone->used_packet);
	if (check_is_free(page, zone->used_packet))
	{
		ft_printf("free\n");
		munmap(page->ptr, page->size);
	}
	else
	{
		ft_printf("leaks!!!!!!!!!\n");
	}
}

void		auto_free(int option)
{
	if (!data)
	{
		ft_printf("please call malloc once before\n");
		return ;
	}
	data->free_status = option;
}

void		free_at_exit(void)
{
	if (!data || !data->pages_size)
		return ;
	
	if (data->free_status)
	{
		check_fogotten_free();
		remove_all_page(data->tiny.page);
		remove_all_page(data->small.page);
		remove_all_page(data->large.page);
		remove_all_page(data->data_page.page);
		ft_printf("all free\n");
	}
	else
	{
		remove_free_page(&data->tiny, data->tiny.page);
		remove_free_page(&data->small, data->small.page);
		remove_free_page(&data->large, data->large.page);
		remove_free_page(&data->data_page, data->data_page.page);
	}
}

void		ft_free(void *ptr)
{
	if (chr_allocation(ptr, &data->tiny))
		return ;
	if (chr_allocation(ptr, &data->small))
		return ;
	if (chr_allocation(ptr, &data->large))
		return ;
	//sortie erreur 2
	ft_printf("malloc: *** error for object %p: pointer being freed was not allocated\n", ptr);
}
