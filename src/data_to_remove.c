/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_to_remove.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 03:26:39 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/06 11:28:32 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"

static void	remove_packet(t_zone *zone, t_mem *packet, t_mem *prec)
{
	if (prec == NULL)
		zone->used_packet = packet->next;
	else
		prec->next = packet->next;
	packet->next = NULL;
	insert_new_packet(&(zone->packet), packet);
}

int			chr_allocation(void *ptr, t_zone *zone)
{
	t_mem	*current_packet;

	current_packet = zone->used_packet;
	if (!current_packet)
		return(0);
	if (current_packet->ptr == ptr)
	{
		remove_packet(zone, current_packet, NULL);
		return(1);
	}
	while (current_packet->next)
	{
		if (current_packet->next->ptr == ptr)
		{
			remove_packet(zone, current_packet->next, current_packet);
			return(1);
		}
		current_packet = current_packet->next;
	}
	return(0);
}
