/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 10:19:04 by tderwedu          #+#    #+#             */
/*   Updated: 2022/01/31 17:58:26 by tderwedu         ###   ########.fr       */
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

# include "NetworkIPC.hpp"

# include <iostream>
# include <vector>
# include <map>

# include "Server.hpp"
# include "Location.hpp"
# include "Request.hpp"
# include "Response.hpp"
# include "NetworkSocket.hpp"

# ifdef OPEN_MAX
	static long	open_max = OPEN_MAX;
# else
	static long	open_max = 0;
# endif
# define OPEN_MAX_GUESS		256
# define POLL_FLAGS			POLLIN | POLLOUT	// POLLERR | POLLHUP | POLLNVAL are allways set

class Webserv
{
private:
	int							_fdInUse;
	std::vector<t_poll>			_pollfd;
	std::vector<Server>			_servers;
	std::vector<NetworkSocket>	_serverSock;
	std::vector<NetworkSocket>	_clientSock;
public:
	Webserv(void);
	~Webserv(void);

	void	setOpenMax(void);
	void	initServers(void);
	void	addServerSocket(int port, int nbr_queue);
	void	newConnection(NetworkSocket const& sock);
	void	addPollfd(int fd_client);
	void	popPollfd(t_poll& pollfd);
};

Webserv::Webserv(void) {}

Webserv::~Webserv(void) {}

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
				std::cerr << "sysconf error for _SC_OPEN_MAX" << std::endl;		//TODO: better error handling
		}
	}
}

void	Webserv::addServerSocket(int port, int nbr_queue)
{
	int			fd_sock;
	int			opt;
	t_poll		new_pollfd;
	socklen_t	addr_len;
	t_addr_in	addr_sock;

	opt = 1;
	if ((fd_sock = socket(AF_INET, SOCK_STREAM, 0)) < 1)
		exit(EXIT_FAILURE);														//TODO: better error handling;
	
	// Set NON)BLOCKING and SO_REUSEADDR
	// Avoid EADDRINUSE -> BIND and LISTEN should not return any error
	fcntl(fd_sock, F_SETFL, O_NONBLOCK);
	setsockopt(fd_sock, SOL_SOCKET, SO_REUSEADDR, (char *)(&opt), sizeof(opt));

	bzero(&addr_sock, sizeof(addr_sock));
	addr_sock.sin_family = AF_INET;
	addr_sock.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_sock.sin_port = htons(port);
	bind(fd_sock, (t_addr *) &addr_sock, sizeof(addr_sock));

	nbr_queue = (nbr_queue > SOMAXCONN ? SOMAXCONN : nbr_queue);
	listen(fd_sock, nbr_queue);

	new_pollfd.fd = fd_sock;
	new_pollfd.events = POLL_FLAGS;
	new_pollfd.revents = 0;
	_pollfd.push_back(new_pollfd);
	++_fdInUse;

	_serverSock.push_back(NetworkSocket(port, fd_sock));
}

void	Webserv::initServers(void) {}

void	Webserv::newConnection(NetworkSocket const& sock)
{
	int			fd_client;
	socklen_t	addr_len;
	t_addr_in	addr_sock;

	addr_len = sizeof(addr_sock);
	if ((fd_client = accept(sock.fd, (t_addr *) &addr_sock, &addr_len)) < 0)
		exit(EXIT_FAILURE);														//TODO: better error handling;
	fcntl(fd_client, F_SETFL, O_NONBLOCK);
	addPollfd(fd_client);
	_clientSock.push_back(NetworkSocket(sock.port, fd_client));
}

void	Webserv::addPollfd(int fd_client)
{
	t_poll		new_pollfd;

	if (_fdInUse == _pollfd.size())
	{
		new_pollfd.fd = fd_client;
		new_pollfd.events = POLL_FLAGS;
		new_pollfd.revents = 0;
		_pollfd.push_back(new_pollfd);
	}
	else
	{
		std::vector<pollfd>::iterator	it = _pollfd.begin();

		while (it != _pollfd.end())
		{
			if (it->fd < 0)
				break ;
			 ++it;
		}
		it->fd = fd_client;
	}
	++_fdInUse;
}

void	Webserv::popPollfd(t_poll&	pollfd)
{
	pollfd.fd = -1;
	--_fdInUse;
}

#endif
