/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 02:18:32 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/06 07:03:33 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"

static void	remove_all_page(t_mem *page, int is_user)
{
	size_t		size;

	size = 0;
	if (!page)
		return ;
	remove_all_page(page->next, is_user);
	munmap(page->ptr, page->size);
}

void		free_at_exit(void)
{
	if (!data || !data->pages_size)
		return ;
	
	remove_all_page(data->tiny.page, 1);
	remove_all_page(data->small.page, 1);
	remove_all_page(data->large.page, 1);
	remove_all_page(data->data_page.page, 0);
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
