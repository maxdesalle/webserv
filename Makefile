# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/18 10:08:26 by maxdesall         #+#    #+#              #
#    Updated: 2022/02/23 19:28:21 by tderwedu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	webserv

# =============================== COMPILATION ================================ #

CC			:= clang++
CFLAGS		:= -Wall -Wextra -Werror -std=c++98

# ================================== FILES =================================== #


MAIN		:= Webserv.cpp
CONFIG		:= ConfigHandler.cpp \
			   Server.cpp \
			   Location.cpp
SOCKETS		:= RequestHandler.cpp \
			   NetworkSocket.cpp \
			   ClientSocket.cpp \
			   Webserv.cpp
HTTP		:= Request.cpp \
			   URI.cpp \
			   Response.cpp \
			   Header.cpp
UTILS		:= Timer.cpp \
			   utils.cpp

# INCLUDES	:= -I ./${SRC_DIR} -I ./${TEST_DIR}

BIN_DIR		:= .bin/
SRC_DIR		:= src/
MAIN_DIR	:= $(addprefix src/, $(MAIN))
UTILS		:= $(addprefix src/utils/, $(UTILS))
CONFIG_DIR	:= $(addprefix src/config/, $(CONFIG))
SOCKETS		:= $(addprefix src/sockets/, $(SOCKETS))
HTTP		:= $(addprefix src/HTTP_Config/, $(HTTP))

SRC			:= $(MAIN_DIR) $(UTILS) $(CONFIG_DIR) $(HTTP) $(SOCKETS)
# SRC			:= $(MAIN_DIR) $(CONFIG_DIR)
OBJ			:= $(subst ${SRC_DIR},${BIN_DIR}, ${SRC:.cpp=.o})

# ================================== RULES =================================== #

$(BIN_DIR)%.o:	$(SRC_DIR)%.cpp
				@mkdir -p $(dir $@)
				$(CC) $(CFLAGS) -c $< -o $@

$(NAME):		$(OBJ)
				@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

all:			$(NAME)

clean:
				@rm -rf $(OBJ)
				@rm -rf *.dSYM

fclean:			clean
				@rm -rf $(BIN_DIR)
				@rm -rf $(NAME)

re:				fclean all

.PHONY:			all clean re fclean
