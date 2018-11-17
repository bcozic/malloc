/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reallocf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 19:41:37 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/17 14:48:26 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"

static void		*no_thread_safe_malloc(size_t size)
{
	void	*ptr;

	if (size == 0)
		return (0);
	if (g_data == NULL)
		if (!init_data((size_t)getpagesize()))
			return (0);
	if (size <= g_data->pages_size)
		ptr = get_alloc(size, &g_data->tiny,
				(g_data->pages_size * 100 / g_data->pages_size));
	else if (size <= g_data->pages_size * 4)
		ptr = get_alloc(size, &g_data->small,
				(g_data->pages_size * 4 * 100 / g_data->pages_size));
	else
		ptr = get_alloc(size, &g_data->large, 0);
	return (ptr);
}

void			*reallocf(void *ptr, size_t size)
{
	t_mem	*packet;
	size_t	original_size;
	void	*new_ptr;

	original_size = 0;
	pthread_mutex_lock(&g_mutex);
	if ((packet = find_packet(ptr, &g_data->tiny)))
		original_size = packet->size;
	else if ((packet = find_packet(ptr, &g_data->small)))
		original_size = packet->size;
	else if ((packet = find_packet(ptr, &g_data->large)))
		original_size = packet->size;
	if (!(ret_free(ptr)))
	{
		pthread_mutex_unlock(&g_mutex);
		return (NULL);
	}
	if (size == 0)
		size = 1;
	new_ptr = no_thread_safe_malloc(size);
	if (new_ptr && new_ptr != ptr && original_size)
		ft_memmove(new_ptr, ptr, original_size);
	pthread_mutex_unlock(&g_mutex);
	return (new_ptr);
}
