/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/12 10:27:14 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/14 18:59:13 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"

static size_t	aff_packet(t_mem *packet)
{
	ft_putstr("0x");
	ft_putsizet((size_t)packet->ptr);
	ft_putstr(" - 0x");
	ft_putsizet((size_t)((char*)packet->ptr
			+ packet->size));
	ft_putstr(": ");
	ft_putnbr((int)(((char*)packet->ptr
			+ packet->size) - (char*)packet->ptr));
	ft_putstr(" octets\n");
	return ((size_t)(((char*)packet->ptr
			+ packet->size) - (char*)packet->ptr));
}

static size_t	aff_page(t_mem *page, t_mem *packet, char *str)
{
	static t_mem	*current_packet = NULL;
	size_t			size;

	size = 0;
	if (current_packet == NULL)
		current_packet = packet;
	if (!current_packet || (char*)current_packet->ptr
			> (char*)page->ptr + page->size)
		return (0);
	ft_putstr(str);
	ft_putsizet((size_t)page->ptr);
	write(1, "\n", 1);
	while (current_packet && (char*)current_packet->ptr
			< (char*)page->ptr + page->size)
	{
		size += aff_packet(current_packet);
		current_packet = current_packet->next;
	}
	return (size);
}

static size_t	find_lower(t_mem *tiny, t_mem *small, t_mem *large)
{
	size_t	size;

	size = 0;
	while (tiny || small || large)
	{
		if (tiny && (!small || tiny->ptr < small->ptr)
				&& (!large || tiny->ptr < large->ptr))
		{
			size += aff_page(tiny, g_data->tiny.used_packet, "TINY : 0x");
			tiny = tiny->next;
		}
		else if (small && (!tiny || small->ptr < tiny->ptr)
				&& (!large || small->ptr < large->ptr))
		{
			size += aff_page(small, g_data->small.used_packet, "SMALL : 0x");
			small = small->next;
		}
		else
		{
			size += aff_page(large,
					g_data->large.used_packet, "LARGE : 0x");
			large = large->next;
		}
	}
	return (size);
}

void			show_alloc_mem(void)
{
	t_mem	*tiny_page;
	t_mem	*small_page;
	t_mem	*large_page;
	size_t	size_total;

	pthread_mutex_lock(&g_mutex);
	tiny_page = g_data->tiny.page;
	small_page = g_data->small.page;
	large_page = g_data->large.page;
	size_total = find_lower(tiny_page, small_page, large_page);
	pthread_mutex_unlock(&g_mutex);
	ft_printf("Total : %ld octets\n", size_total);
}
