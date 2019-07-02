# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2018/11/05 12:24:50 by mtaquet      #+#   ##    ##    #+#        #
#    Updated: 2019/06/06 14:07:00 by mtaquet     ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

NAME = minishell
LIBDIR = libft/
SRCDIR = srcs/
INCDIR = includes/
LIBFILES = libft.a
SRCFILES = main.c\
		   builtin1.c\
		   builtin2.c\
		   exe.c\
		   init.c\
		   command.c\

INCFILES = minishell.h
LIB = $(addprefix $(LIBDIR),$(LIBFILES))
SRC = $(addprefix $(SRCDIR),$(SRCFILES))
INC = $(addprefix $(INCDIR),$(INCFILES)) libft/includes/libft.h
OBJ = $(SRC:.c=.o)
FLAG = -Wall -Werror -Wextra -I includes -I libft/includes

all: lib $(NAME)

%.o: %.c $(INC)
	gcc -c -o $@ $< $(FLAG)

lib: $(LIBDIR)
	@make -C $(LIBDIR)

$(NAME): $(LIB) $(OBJ) $(INC)
	gcc -o $@ $(OBJ) $(FLAG) -ltermcap $(LIB) 

clean:
	rm -f $(OBJ)
	make clean -C $(LIBDIR)

fclean: clean
	rm -f $(NAME)
	rm -f $(LIB)

re: fclean all
