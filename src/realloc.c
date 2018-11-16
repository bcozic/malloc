/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 06:18:55 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/16 21:44:57 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"

t_mem			*find_packet(void *ptr, t_zone *zone)
{
	t_mem	*current_packet;

	current_packet = zone->used_packet;
	if (!current_packet)
		return (NULL);
	while (current_packet)
	{
		if (current_packet->ptr == ptr)
		{
			return (current_packet);
		}
		current_packet = current_packet->next;
	}
	return (NULL);
}

static int		join_packet(t_mem *packet, t_mem *current,
		t_mem *prec, t_zone *zone)
{
	packet->size += current->size;
	if (!prec)
		zone->packet = current->next;
	else
		prec->next = current->next;
	remove_struct_packet(current);
	return (1);
}

static int		redimmention_ptr(t_mem *packet, size_t size, t_zone *zone)
{
	t_mem	*current;
	t_mem	*prec;
	size_t	size_to_add;

	if (packet->size >= size)
		return (1);
	current = zone->packet;
	prec = NULL;
	while (current && current->ptr < packet->ptr)
	{
		prec = current;
		current = current->next;
	}
	if (current && (char*)current->ptr == (char*)packet->ptr + packet->size)
	{
		if (current->size + packet->size < size)
			return (0);
		size_to_add = size - packet->size;
		size_to_add = ((size_to_add / zone->size_packet_zone)
			+ (size_to_add % zone->size_packet_zone ? 1 : 0))
			* zone->size_packet_zone;
		if (div_packet(current, size_to_add))
			return (join_packet(packet, current, prec, zone));
	}
	return (0);
}

static size_t	is_redimention(void *ptr, size_t size)
{
	t_mem	*packet;

	if ((packet = find_packet(ptr, &g_data->tiny)))
	{
		if (redimmention_ptr(packet, size, &g_data->tiny))
			return (0);
	}
	else if ((packet = find_packet(ptr, &g_data->small)))
	{
		if (redimmention_ptr(packet, size, &g_data->small))
			return (0);
	}
	else if ((packet = find_packet(ptr, &g_data->large)))
	{
		if (redimmention_ptr(packet, size, &g_data->large))
			return (0);
	}
	else
		return (1);
	return (packet->size);
}

void			*realloc(void *ptr, size_t size)
{
	void	*new_ptr;
	size_t	old_size;

	old_size = 0;
	pthread_mutex_lock(&g_mutex);
	if (size == 0)
		size = 1;
	if (ptr && !(old_size = is_redimention(ptr, size)))
	{
		pthread_mutex_unlock(&g_mutex);
		return (ptr);
	}
	if (old_size == 1)
	{
		pthread_mutex_unlock(&g_mutex);
		return (NULL);
	}
	pthread_mutex_unlock(&g_mutex);
	if (!(new_ptr = malloc(size)))
		return (NULL);
	ft_memmove(new_ptr, ptr, old_size);
	free(ptr);
	return (new_ptr);
}
