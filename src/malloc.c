/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/02 22:35:05 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/14 19:36:15 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"

t_data			*g_data = NULL;
pthread_mutex_t	g_mutex = PTHREAD_MUTEX_INITIALIZER;

int		init_data(size_t pages_size)
{
	void		*map;

	if (pages_size <= 0)
		return (0);
	map = mmap(NULL, pages_size * 3, PROT_READ | PROT_WRITE,
			MAP_ANON | MAP_PRIVATE, -1, 0);
	g_data = (t_data*)map;
	g_data->pages_size = pages_size;
	g_data->tiny.size_packet_zone = TINY_PK;
	g_data->small.size_packet_zone = SMALL_PK;
	g_data->large.size_packet_zone = pages_size;
	g_data->data_page.page = (t_mem*)(void*)((char*)map + sizeof(t_data));
	g_data->data_page.page->ptr = map;
	g_data->data_page.page->size = pages_size * 3;
	g_data->data_page.page->next = NULL;
	g_data->data_page.packet = (t_mem*)(void*)((char*)map + sizeof(t_data)
			+ sizeof(t_mem));
	g_data->data_page.packet->ptr = (char*)map + sizeof(t_data)
			+ sizeof(t_mem) + sizeof(t_mem);
	g_data->data_page.packet->size = (size_t)((char*)map
			- (char*)g_data->data_page.packet->ptr) + (pages_size * 3);
	g_data->data_page.packet->next = NULL;
	g_data->allocate_size = g_data->pages_size;
	getrlimit(RLIMIT_MEMLOCK, &g_data->rlimit_memlock);
	return (1);
}

void	*malloc(size_t size)
{
	void	*ptr;

	if (size == 0)
		return (0);
	pthread_mutex_lock(&g_mutex);
	if (g_data == NULL)
		if (!init_data((size_t)getpagesize()))
		{
			pthread_mutex_unlock(&g_mutex);
			return (0);
		}
	if (size <= g_data->pages_size)
		ptr = get_alloc(size, &g_data->tiny,
				(g_data->pages_size * 100 / g_data->pages_size));
	else if (size <= g_data->pages_size * 4)
		ptr = get_alloc(size, &g_data->small,
				(g_data->pages_size * 4 * 100 / g_data->pages_size));
	else
		ptr = get_alloc(size, &g_data->large, 0);
	pthread_mutex_unlock(&g_mutex);
	return (ptr);
}
