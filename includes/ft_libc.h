/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_libc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/02 22:37:53 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/09 15:38:33 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <pthread.h>

#ifndef FT_LIBC_H
# define FT_LIBC_H

# define TINY_ZONE	256
# define SMALL_ZONE	4096
# define TINY_PK	16
# define SMALL_PK	64

typedef struct				s_mem
{
	void					*ptr;
	size_t					size;
	struct s_mem			*next;
}							t_mem;

typedef struct				s_zone
{
	t_mem					*page;
	t_mem					*packet;
	t_mem					*used_packet;
	size_t					size_packet_zone;
}							t_zone;

typedef struct				s_data
{
	size_t					pages_size;
	t_zone					tiny;
	t_zone					small;
	t_zone					large;
	t_zone					data_page;
	struct rlimit			rlimit_memlock;
	size_t					allocate_size;
}							t_data;

extern	t_data* data;
extern pthread_mutex_t mutex;

void						*ft_malloc(size_t size);
void						*get_new_struct(size_t size);
void						*get_alloc(size_t size, t_zone *zone, size_t nb_page);
t_mem						*get_new_page(t_zone *zone, size_t nb_pages);
void						remove_struct_packet(t_mem *packet);
t_mem						*insert_new_packet(t_mem **list_ptr, t_mem *new_ptr);


void						ft_free(void *ptr);
void						unmap_at_exit(void) __attribute__ ((destructor));
int							chr_allocation(void *ptr, t_zone *zone);

void						*f_realloc(void *ptr, size_t size);

void	check_address(void *to_check, t_zone *zone, char *str, size_t size);
void	check_page_size(char *str);

#endif
