/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/02 22:39:26 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/09 16:01:24 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"
#include <stdlib.h>
#include <unistd.h>


static void	*multi_malloc(void* ret)
{
	(void)ret;
	int i = 1000;
	char *test;
	char *test2;
	char *test3;
	char *test4;
	char *test5;
	char *test6;

	while(i--)
	{
		if (!(test = ft_malloc(10)))
		{
			ft_printf("ERROR\n");
			return (ret);
		}
		if (!(test2 = ft_malloc(100)))
		{
			ft_printf("ERROR\n");
			return (ret);
		}
		if (!(test3 = ft_malloc(20)))
		{
			ft_printf("ERROR\n");
			return (ret);
		}
		if (!(test4 = ft_malloc(48)))
		{
			ft_printf("ERROR\n");
			return (ret);
		}
		if (!(test5 = ft_malloc(70)))
		{
			ft_printf("ERROR\n");
			return (ret);
		}
		if (!(test6 = ft_malloc(800)))
		{
			ft_printf("ERROR\n");
			return (ret);
		}
		ft_free(test4);
		ft_free(test2);
		ft_free(test);
		ft_free(test3);
		ft_free(test6);
		ft_free(test5);
	}
	return (ret);
}
// malloc ./a.out  14.24s user 2.76s system 182% cpu 9.340 total
// ft_malloc ./a.out  16.07s user 7.05s system 212% cpu 10.856 total
int	main()
{
	int i = 100;
	pthread_t	thread[100];
	void		*value[100];
	int nb[100];
	while(i--)
	{
		nb[i] = i;
		if (pthread_create(&thread[i], NULL, &multi_malloc, &nb[i]))
			break ;
		//ft_printf("new thread %d\n", i);
	}
	i = 100;
	while(i--)
		pthread_join(thread[i], &value[i]);
// 	char	*test;
// size_t i = 10000;
// while(i--)
// {
// 	//ft_printf("b %d\n", i);
// 		test = (char*)malloc(10);
// 		if (!test)
// 		{
// 			ft_printf("ERROR\n");
// 			return 0;
// 		}
// 		//ft_printf("addr test %p\n", test);
// 		test = realloc(test, 800);
// 		if (!test)
// 		{
// 			ft_printf("ERROR\n");
// 			return 0;
// 		}
// 		ft_memcpy(test, "test", 5);
// 		//ft_printf("%s   %p\n", test, test);
// 		free(test);

// 		test = (char*)malloc(10);
// 		if (!test)
// 		{
// 			ft_printf("ERROR\n");
// 			return 0;
// 		}
// 		//ft_printf("%s   %p\n", test, test);
// 		free(test);

// 		test = malloc(88);
// 				if (!test)
// 		{
// 			ft_printf("ERROR\n");
// 			return 0;
// 		}
// 		free(test);
// }
	return(0);
}
