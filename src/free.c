/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 02:18:32 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/09 16:08:29 by bcozic           ###   ########.fr       */
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
	check_page_size("check_fogotten_free");
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
	//if (size)
		//ft_printf("WARNING you will have %d leaks\nI'm kind, I free it for you\n", size);
}

void		unmap_at_exit(void)
{
	if (!data || !data->pages_size)
		return ;
	
	check_fogotten_free();
	remove_all_page(data->tiny.page);
	remove_all_page(data->small.page);
	remove_all_page(data->large.page);
	remove_all_page(data->data_page.page);
	ft_printf("all free\n");
}

void		ft_free(void *ptr)
{

	pthread_mutex_lock(&mutex);
	//ft_printf("free\n");
	if (!data)
	{
		//ft_printf("malloc: *** error for object %p: pointer being freed was not allocated\n", ptr);
		pthread_mutex_unlock(&mutex);
		return ;
	}
	check_page_size("ft_free");
	if (chr_allocation(ptr, &data->tiny))
	{
		pthread_mutex_unlock(&mutex);
		return ;
	}
	if (chr_allocation(ptr, &data->small))
	{
		pthread_mutex_unlock(&mutex);
		return ;
	}
	if (chr_allocation(ptr, &data->large))
	{
		pthread_mutex_unlock(&mutex);
		return ;
	}
	//sortie erreur 2
	//ft_printf("malloc: *** error for object %p: pointer being freed was not allocated\n", ptr);
	pthread_mutex_unlock(&mutex);
}
