/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_libc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/02 22:37:53 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/16 19:42:43 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LIBC_H
# define FT_LIBC_H

# include "libft.h"
# include <unistd.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <pthread.h>

# define TINY_PK	16
# define SMALL_PK	512

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

extern						t_data* g_data;
extern pthread_mutex_t		g_mutex;

void						*malloc(size_t size);
int							init_data(size_t pages_size);
void						*get_new_struct(size_t size);
void						*get_alloc(size_t size, t_zone *zone,
									size_t nb_page);
t_mem						*get_new_page(t_zone *zone, size_t nb_pages);
void						remove_struct_packet(t_mem *packet);
t_mem						*insert_new_packet(t_mem **list_ptr,
									t_mem *new_ptr);
void						*get_mem(t_zone *zone, t_mem *previous,
									t_mem *packet, size_t size);
void						add_used_packet(t_zone *zone, t_mem *packet);
int							div_packet(t_mem *packet, size_t size);
void						free(void *ptr);
void						unmap_at_exit(void) __attribute__((destructor));
int							chr_allocation(void *ptr, t_zone *zone);
void						*realloc(void *ptr, size_t size);
t_mem						*find_packet(void *ptr, t_zone *zone);
int							ret_free(void *ptr);
void						*calloc(size_t count, size_t size);
void						*valloc(size_t size);
t_mem						*cut_packet(t_mem *packet, void *ptr);
void						*reallocf(void *ptr, size_t size);
void						show_alloc_mem(void);

#endif
