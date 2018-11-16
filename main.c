/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/02 22:39:26 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/16 19:33:35 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"
#include "libft.h"
#include <stdlib.h>
#include <unistd.h>


// static void	*multi_malloc(void* ret)
// {
// 	(void)ret;
// 	int i = 1000;
// 	char *test;
// 	char *test2;
// 	char *test3;
// 	char *test4;
// 	char *test5;
// 	char *test6;

// 	while(i--)
// 	{
// 		if (!(test = valloc(10)))
// 		{
// 			ft_printf("ERROR\n");
// 			return (ret);
// 		}
// 		if ((size_t)test % 4096)
// 			ft_printf("test1 fail\n");
// 		if (!(test2 = valloc(100)))
// 		{
// 			ft_printf("ERROR\n");
// 			return (ret);
// 		}
// 		if ((size_t)test2 % 4096)
// 			ft_printf("test2 fail\n");
// 		if (!(test3 = malloc(20)))
// 		{
// 			ft_printf("ERROR\n");
// 			return (ret);
// 		}
// 		if (!(test4 = malloc(48)))
// 		{
// 			ft_printf("ERROR\n");
// 			return (ret);
// 		}
// 		if (!(test5 = valloc(600)))
// 		{
// 			ft_printf("ERROR\n");
// 			return (ret);
// 		}
// 		if ((size_t)test5 % 4096)
// 			ft_printf("test5 fail\n");
// 		if (!(test6 = malloc(8000)))
// 		{
// 			ft_printf("ERROR\n");
// 			return (ret);
// 		}
// 		free(test4);
// 		free(test2);
// 		free(test);
// 		free(test3);
// 		free(test6);
// 		free(test5);
// 	}
// 	return (ret);
// }
// malloc ./a.out  14.24s user 2.76s system 182% cpu 9.340 total
// ft_malloc ./a.out  16.07s user 7.05s system 212% cpu 10.856 total
int	main()
{

	ft_putstr("begin main\n");
	// int i = 100;
	// pthread_t	thread[100];
	// void		*value[100];
	// int nb[100];
	// while(i--)
	// {
	// 	nb[i] = i;
	// 	if (pthread_create(&thread[i], NULL, &multi_malloc, &nb[i]))
	// 		break ;
	// 	//ft_printf("new thread %d\n", i);
	// }
	// i = 100;
	// while(i--)
	// 	pthread_join(thread[i], &value[i]);

		char *test;
		test = malloc(10);
		test = realloc(test, 1);
		ft_putstr("test\n");
		ft_printf("%p\n", test);
		//test[0] = 42;
		free(test);
	return(0);
}
