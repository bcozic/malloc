/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/02 22:35:05 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/09 16:09:15 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"

t_data				*data = NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static int	init_data(void)
{
	size_t		pages_size;
	void		*map;

	pages_size = (size_t)getpagesize();
	if (pages_size <= 0)
		return (0);
	map = mmap(NULL, pages_size * 3, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	//ft_printf("mmap 3 data pages\n");
	
	data = (t_data*)map;
	ft_bzero(map, pages_size);
	data->pages_size = pages_size;
	data->tiny.size_packet_zone = TINY_PK;
	data->small.size_packet_zone = SMALL_PK;
	data->large.size_packet_zone = pages_size;
	data->data_page.page = (t_mem*)(void*)((char*)map + sizeof(t_data));
	data->data_page.page->ptr = map;
	data->data_page.page->size = pages_size * 3;
	data->data_page.packet = (t_mem*)(void*)((char*)map + sizeof(t_data) + sizeof(t_mem));
	data->data_page.packet->ptr = (char*)map + sizeof(t_data) + sizeof(t_mem) + sizeof(t_mem);
	data->data_page.packet->size = (size_t)((char*)map - (char*)data->data_page.packet->ptr) + (pages_size * 3);
	data->data_page.packet->next = NULL;
	data->allocate_size = data->pages_size;
	getrlimit(RLIMIT_MEMLOCK, &data->rlimit_memlock);
	//data->rlimit_memlock.rlim_cur = 1260000;

	//to delete
	check_address(data, &data->data_page, "Error init data", sizeof(t_data));
	check_address(data->data_page.page, &data->data_page, "Error init data->data_page.page", sizeof(t_mem));
	check_address(data->data_page.page->ptr, &data->data_page, "Error init data->data_page.page", data->data_page.page->size);
	check_address(data->data_page.packet, &data->data_page, "Error init data->data_page.packet", sizeof(t_mem));
	check_address(data->data_page.packet->ptr, &data->data_page, "Error init data->data_page.packet->ptr", data->data_page.packet->size);
	//end delete
	return (1);
}

void		*ft_malloc(size_t size)
{
	void	*ptr;

	pthread_mutex_lock(&mutex);
	//ft_printf("malloc\n");

	if (data == NULL)
		if (!init_data())
		{
			pthread_mutex_unlock(&mutex);
			ft_printf("error init\n");
			return (0);
		}
	check_page_size("ft_malloc");
	if (size <= TINY_ZONE)
		ptr = get_alloc(size, &data->tiny, (TINY_ZONE * 100 / data->pages_size));
	else if (size <= SMALL_ZONE)
		ptr = get_alloc(size, &data->small, (SMALL_ZONE * 100 / data->pages_size));
	else
		ptr = get_alloc(size, &data->large, 0);
	pthread_mutex_unlock(&mutex);
	return(ptr);
}
