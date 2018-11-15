# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bcozic <bcozic@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/11/07 18:10:54 by bcozic            #+#    #+#              #
#    Updated: 2018/11/14 19:01:00 by bcozic           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc$(HOSTTYPE).so

MAIN_TEST = a.out														####

CFLAGS = -Wall -Wextra -Werror -Weverything -g

NO_COLOR=\x1b[0m

GREEN=\x1b[32;01m

RED=\x1b[31;01m

SRC = malloc.c data_page.c get_alloc.c free.c\
	  data_to_remove.c realloc.c calloc.c show_alloc_mem.c\
	  valloc.c fnc_packets.c



PATH_LIBFT = libft/

PATH_PRINTF = $(PATH_LIBFT)ft_printf/

SRC_LIBFT = ft_abs.c ft_nbrlen.c ft_isint.c ft_memset.c ft_bzero.c\
			ft_memcpy.c ft_memccpy.c ft_memmove.c ft_memchr.c\
			ft_memcmp.c ft_strlen.c ft_tab_strlen.c ft_strdup.c\
			ft_strcpy.c ft_strncpy.c ft_strcat.c ft_strncat.c\
			ft_strlcat.c ft_strchr.c ft_strrchr.c ft_strstr.c\
			ft_strnstr.c ft_strcmp.c ft_strncmp.c ft_atoi.c\
			ft_atoi_unsigned.c ft_isalpha.c ft_isdigit.c ft_issignnb.c\
			ft_isalnum.c ft_isascii.c ft_isprint.c ft_isspace.c\
			ft_isnum.c ft_toupper.c ft_tolower.c ft_isstr_f.c\
			ft_isstr_forf.c ft_memalloc.c ft_memdel.c ft_strnew.c\
			ft_strdel.c ft_strclr.c ft_striter.c ft_striteri.c\
			ft_strmap.c ft_strmapi.c ft_strequ.c ft_strnequ.c\
			ft_strsub.c ft_strjoin.c ft_strjoinflush.c ft_strnjoinflush.c\
			ft_strtrim.c ft_strsplit.c ft_itoa.c ft_putchar.c\
			ft_putstr.c ft_putendl.c ft_putnbr.c ft_putchar_fd.c\
			ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_lstnew.c\
			ft_lstdelone.c ft_lstdel.c ft_lstadd.c ft_lstiter.c\
			ft_lstmap.c ft_lstlen.c ft_realloc.c ft_strup.c\
			get_next_line.c ft_free_and_reset.c ft_putsizet.c

SRC_PRINTF = ft_printf.c chrflags.c add_arg.c apply_flag.c\
			 convert_nb.c flag_cara.c llitoa_base_mod.c ullitoa_base_mod.c\
			 unicode.c\

INCLUDE = includes/ft_libc.h

OBJ = $(addprefix obj/, $(SRC:.c=.o))

OBJ_LIBFT = $(addprefix $(PATH_LIBFT)obj/, $(SRC_LIBFT:.c=.o))

OBJ_PRINTF = $(addprefix $(PATH_PRINTF)obj/, $(SRC_PRINTF:.c=.o))

all : obj lib $(MAIN_TEST) $(NAME)

lib :
	make -C libft

obj :
	mkdir obj

obj/%.o: src/%.c $(INCLUDE)
	gcc $(CFLAGS) -c $< -o $@ -I includes -I $(PATH_LIBFT)includes -I includes

$(NAME) : $(OBJ)
	gcc $(CFLAGS) -shared -o $(NAME) $(OBJ) $(OBJ_LIBFT) $(OBJ_PRINTF)
	rm -f libft_malloc.so
	ln -s $(NAME) libft_malloc.so
	@echo "$(GREEN)$(NAME) OK$(NO_COLOR)"

$(MAIN_TEST) : main.c $(NAME)																			####
	gcc $(CFLAGS) main.c -I includes -I libft/includes libft_malloc.so							####
	@echo "$(GREEN)$(MAIN_TEST) OK$(NO_COLOR)"

clean :
	rm -f $(OBJ)
	make -C libft clean
	@echo "$(RED)$(NAME) obj Deleted$(NO_COLOR)"

fclean : clean
	rm -f $(NAME)
	rm -f libft_malloc.so
	rm -f $(MAIN_TEST)																			####
	@echo "$(RED)$(NAME) Deleted$(NO_COLOR)"
	@echo "$(RED)$(MAIN_TEST) Deleted$(NO_COLOR)"

re : fclean all

.PHONY : re all clean fclean
