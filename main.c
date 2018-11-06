/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/02 22:39:26 by bcozic            #+#    #+#             */
/*   Updated: 2018/11/06 12:31:08 by bcozic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_libc.h"
//#include <stdlib.h>

int	main()
{
	char	*test;

	test = (char*)ft_malloc(10);
	if (!test)
	{
		ft_printf("ERROR\n");
		return 0;
	}
	ft_memcpy(test, "test", 5);
	ft_printf("%s   %p\n", test, test);
	test = (char*)ft_malloc(10);
	ft_printf("%s   %p\n", test, test);
	test = ft_malloc(88888888);

	ft_free(test);
	return(0);
}
