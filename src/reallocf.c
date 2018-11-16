/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reallocf.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/16 19:41:37 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/16 21:43:26 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"

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
	pthread_mutex_unlock(&g_mutex);
	new_ptr = malloc(size);
	if (new_ptr && new_ptr != ptr && original_size)
		ft_memmove(new_ptr, ptr, original_size);
	return (new_ptr);
}
