# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maxdesalle <mdesalle@student.s19.be>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/18 10:08:26 by maxdesall         #+#    #+#              #
#    Updated: 2022/02/01 11:15:35 by mdesalle         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	webserv

# =============================== COMPILATION ================================ #

CC			:= clang++
TEST		:= clang++ -fsanitize=address -g3
CFLAGS		:= -Wall -Wextra -Werror -std=c++98

# ================================== FILES =================================== #

OBJ			:= $(SRC:.cpp=.o)

MAIN		:= Webserv.cpp
CONFIG		:= ConfigHandler.cpp \
			   Server.cpp \
			   Location.cpp

MAIN_DIR	:= $(addprefix src/, $(MAIN))
CONFIG_DIR	:= $(addprefix src/config/, $(CONFIG))

SRC			:= $(MAIN_DIR) $(CONFIG_DIR)

# ================================== RULES =================================== #

all:		$(NAME)

$(NAME):
			@$(CC) $(CFLAGS) $(SRC) -o $(NAME)

test:		
			@$(TEST) $(CFLAGS) $(SRC) -o $(NAME)

clean:
			@rm -rf $(OBJ)
			@rm -rf *.dSYM

fclean:
			@rm -rf $(OBJ)
			@rm -rf $(NAME)
			@rm -rf *.dSYM

re:			fclean all

.PHONY:		all clean re fclean
