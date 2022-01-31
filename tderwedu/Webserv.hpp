/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 10:19:04 by tderwedu          #+#    #+#             */
/*   Updated: 2022/01/31 12:58:36 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSER_HPP
# define WEBSERV_HPP

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

# include <iostream>
# include <vector>
# include <map>
# include "Server.hpp"
# include "Location.hpp"
# include "Request.hpp"
# include "Response.hpp"

# ifdef OPEN_MAX
	static long	open_max = OPEN_MAX;
# else
	static long	open_max = 0;
# endif
# define OPEN_MAX_GUESS	256
# define POLL_FLAGS		POLLIN | POLLOUT | POLLERR | POLLHUP | POLLNVAL

typedef struct sockaddr_in	t_addr_in;
typedef struct sockaddr		t_addr;
typedef struct pollfd		t_poll;	// Used by `poll` function

class Webserv
{
private:
	std::vector<Server>		_servers;
	std::vector<t_poll>		_poll_lst;
	std::vector<Request>	_requests;
	std::vector<Response>	_responses;
public:
	Webserv();
	~Webserv();

	void	setOpenMax(void);
	void	addListenSocket(int port, int nbr_queue);
};

Webserv::Webserv(/* args */)
{
}

Webserv::~Webserv()
{
}

void	Webserv::setOpenMax(void)
{
	if (!open_max)
	{
		errno = 0;
		if ((open_max = sysconf(_SC_OPEN_MAX)) < 0)
		{
			if (errno == 0)
				open_max = OPEN_MAX_GUESS;
			else
				std::cerr << "sysconf error for _SC_OPEN_MAX" << std::endl;	//TODO: better error handling
		}
	}
}

void	Webserv::addListenSocket(int port, int nbr_queue)
{
	int			fd_sock;
	int			opt;
	t_poll		poll_fd;
	socklen_t	addr_len;
	t_addr_in	addr_sock;

	opt = 1;
	fd_sock = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(fd_sock, F_SETFL, O_NONBLOCK);
	if(setsockopt(fd_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )  
	{
		perror("setsockopt");	//TODO: better error handling
		exit(EXIT_FAILURE);		//TODO: better error handling
	}
	bzero(&addr_sock, sizeof(addr_sock));
	addr_sock.sin_family = AF_INET;
	addr_sock.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_sock.sin_port = htons(port);
	bind(fd_sock, (t_addr *) &addr_sock, sizeof(addr_sock));
	if (nbr_queue > SOMAXCONN)
		nbr_queue = SOMAXCONN;
	listen(fd_sock, nbr_queue);
	poll_fd.fd = fd_sock;
	poll_fd.events = POLL_FLAGS;
	poll_fd.revents = 0;
	_poll_lst.push_back(poll_fd);
}

#endif
