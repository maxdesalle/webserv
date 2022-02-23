/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkIPC.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 16:05:34 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/23 11:06:03 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORKIPC_HPP
# define NETWORKIPC_HPP

# include <unistd.h>		// fcntl, sysconf
# include <string.h>		// bzero
# include <errno.h>
# include <fcntl.h>			// fcntl
# include <sys/socket.h>	// socket(), shutdown(), bind(), listen(), accept(), recv(), send()
# include <arpa/inet.h>		// htons(), htonl(), ntohs(), ntohl()
# include <netinet/in.h>
# include <sys/time.h> 		// struct timeval
# include <limits.h>		// for OPEN_MAX
# include <poll.h>

# include "../utils/Timer.hpp"
# include "../utils/utils.hpp"

#include "../HTTP_Config/Header.hpp"
#include "../HTTP_Config/Request.hpp"
#include "../HTTP_Config/Response.hpp"
#include "../HTTP_Config/URI.hpp"

#include "RequestHandler.hpp"
#include "NetworkSocket.hpp"
#include "ClientSocket.hpp"
#include "Webserv.hpp"

typedef struct sockaddr_in	t_sockaddr_in;	// Used by `bind` function
typedef struct sockaddr		t_sockaddr;		// Used by `bind` function
typedef struct pollfd		t_poll;			// Used by `poll` function

#endif
