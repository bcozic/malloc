/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 02:18:32 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/14 19:50:40 by bcozic           ###   ########.fr       */
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

static void	check_fogotten_free(void)
{
	t_mem	*current;
	size_t	size;

	size = 0;
	current = g_data->tiny.used_packet;
	while (current)
	{
		size += current->size;
		current = current->next;
	}
	current = g_data->small.used_packet;
	while (current)
	{
		size += current->size;
		current = current->next;
	}
	current = g_data->large.used_packet;
	while (current)
	{
		size += current->size;
		current = current->next;
	}
}

void		unmap_at_exit(void)
{
	if (!g_data || !g_data->pages_size)
		return ;
	check_fogotten_free();
	remove_all_page(g_data->tiny.page);
	remove_all_page(g_data->small.page);
	remove_all_page(g_data->large.page);
	remove_all_page(g_data->data_page.page);
}

int			ret_free(void *ptr)
{
	if (!ptr)
		return (1);
	else if (!g_data)
		return (0);
	else if (chr_allocation(ptr, &g_data->tiny))
		return (1);
	else if (chr_allocation(ptr, &g_data->small))
		return (1);
	else if (chr_allocation(ptr, &g_data->large))
		return (1);
	else
		return (0);
}

void		free(void *ptr)
{
	if (!ptr)
		return ;
	pthread_mutex_lock(&g_mutex);
	if (!g_data)
	{
		pthread_mutex_unlock(&g_mutex);
		return ;
	}
	if (chr_allocation(ptr, &g_data->tiny))
	{
		pthread_mutex_unlock(&g_mutex);
		return ;
	}
	if (chr_allocation(ptr, &g_data->small))
	{
		pthread_mutex_unlock(&g_mutex);
		return ;
	}
	if (chr_allocation(ptr, &g_data->large))
	{
		pthread_mutex_unlock(&g_mutex);
		return ;
	}
	pthread_mutex_unlock(&g_mutex);
}
