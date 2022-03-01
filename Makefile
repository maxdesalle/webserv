# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/18 10:08:26 by maxdesall         #+#    #+#              #
#    Updated: 2022/03/01 14:45:05 by tderwedu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:=	webserv

# =============================== COMPILATION ================================ #

CC				:= clang++
TEST			:= clang++ -fsanitize=address -g3
CFLAGS			:= -Wall -Wextra -std=c++98

# ================================== FILES =================================== #

OBJ				:= $(SRC:.cpp=.o)

MAIN			:= Webserv.cpp
CONFIG			:= ConfigHandler.cpp \
				   Server.cpp \
				   Location.cpp
HTTP_CONFIG		:= Response.cpp

MAIN_DIR		:= $(addprefix src/, $(MAIN))
CONFIG_DIR		:= $(addprefix src/config/, $(CONFIG))
HTTP_CONFIG_DIR := $(addprefix src/HTTP_CONFIG/, $(HTTP_CONFIG))

SRC				:= $(MAIN_DIR) $(CONFIG_DIR) $(HTTP_CONFIG_DIR)

# ================================== RULES =================================== #

all:			$(NAME)

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

re:				fclean all

.PHONY:			all clean re fclean
