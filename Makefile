# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/01/18 10:08:26 by maxdesall         #+#    #+#              #
#    Updated: 2022/03/14 15:29:45 by tderwedu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= webserv
DEBUG			:= 0

# =============================== COMPILATION ================================ #

CC			:= clang++
CFLAGS		:= -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3

# ================================== FILES =================================== #

BIN_DIR		:= .bin/
SRC_DIR		:= src/
INC_DIR		:= include/

MAIN		:= Webserv.cpp
CONFIG		:= ConfigHandler.cpp \
			   Server.cpp \
			   Location.cpp
SOCKETS		:= ListenSocket.cpp \
			   ClientSocket.cpp \
			   Webserv.cpp
HTTP		:= Request.cpp \
			   URI.cpp \
			   Response.cpp \
			   Header.cpp \
			   Value.cpp
UTILS		:= Timer.cpp \
			   utils.cpp
CGI			:= CommonGatewayInterface.cpp \

INCLUDES	= -I ./${INC_DIR}
INCLUDES	+= -I $(addprefix $(INC_DIR), config)
INCLUDES	+= -I $(addprefix $(INC_DIR), HTTP_Config)
INCLUDES	+= -I $(addprefix $(INC_DIR), sockets)
INCLUDES	+= -I $(addprefix $(INC_DIR), cgi)
INCLUDES	+= -I $(addprefix $(INC_DIR), utils)

MAIN_DIR	:= $(addprefix src/, $(MAIN))
CONFIG		:= $(addprefix src/config/, $(CONFIG))
HTTP		:= $(addprefix src/HTTP_Config/, $(HTTP))
SOCKETS		:= $(addprefix src/sockets/, $(SOCKETS))
CGI			:= $(addprefix src/cgi/, $(CGI))
UTILS		:= $(addprefix src/utils/, $(UTILS))

SRC			:= $(MAIN_DIR) $(CONFIG) $(HTTP) $(SOCKETS) $(CGI) $(UTILS)
OBJ			:= $(subst ${SRC_DIR},${BIN_DIR}, ${SRC:.cpp=.o})

# ================================== RULES =================================== #

$(BIN_DIR)%.o:	$(SRC_DIR)%.cpp
				@mkdir -p $(dir $@)
ifeq ($(DEBUG), 1)
				$(CC) $(CFLAGS) -D DEBUG -c $< -o $@
else
				$(CC) $(CFLAGS) -c $< -o $@
endif

$(NAME):		$(OBJ)
ifeq ($(DEBUG), 1)
				@$(CC) $(CFLAGS) -D DEBUG $(OBJ) -o $(NAME)
else
				@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
endif

all:			$(NAME)

clean:
				@rm -rf $(OBJ)
				@rm -rf *.dSYM

fclean:			clean
				@rm -rf $(BIN_DIR)
				@rm -rf $(NAME)

re:				fclean all

.PHONY:			all clean re fclean
