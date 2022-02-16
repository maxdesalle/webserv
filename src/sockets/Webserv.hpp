/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 10:19:04 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/16 12:49:30 by tderwedu         ###   ########.fr       */
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
# include "NetworkSocket.hpp"
# include "ClientSocket.hpp"
# include "../HTTP_Config/Request.hpp"
# include "../HTTP_Config/Response.hpp"
# include "../utils/Timer.hpp"
# include "../utils/utils.hpp"

# ifdef OPEN_MAX
	static long	open_max = OPEN_MAX;
# else
	static long	open_max = 0;
# endif
# define OPEN_MAX_GUESS		256
# define POLL_FLAGS			POLLIN | POLLOUT	// POLLERR | POLLHUP | POLLNVAL are allways set

typedef std::vector<Server>					cont_server;
typedef std::vector<NetworkSocket>			cont_netSock;
typedef std::vector<ClientSocket>			cont_cliSock;
typedef cont_netSock::iterator				it_netSock;
typedef cont_cliSock::iterator				it_cliSock; // TODO: Change to deque

class Webserv
{
private:
	static std::string const	_validMethod[4];
	int							_fdInUse;
	std::vector<t_poll>			_pollfd;
	cont_server					_servers;
	cont_netSock				_serverSocks;
	cont_cliSock				_clientSocks;
	
public:
	Webserv(void);
	~Webserv(void);

	void				setOpenMax(void);

	const cont_server&	getServers(void);
	
	void				addServerSocket(int port, int nbr_queue);

	void				checkServerSockets(void);
	void				checkClientSockets(void);
	void				reapClosedClientSock(void);
	
	t_poll&				addPollfd(int fd_client);
	void				popPollfd(t_poll& pollfd);

	int					isValidRequest(const std::string& method);

};

std::string const Webserv::_validMethod[4] = {"GET", "HEAD", "POST", "DELETE"};

Webserv::Webserv(void) {}

Webserv::~Webserv(void)
{
	_pollfd.clear();
	_servers.clear();
	_serverSocks.clear();
	_clientSocks.clear();
}

void				Webserv::setOpenMax(void)
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

const cont_server&	Webserv::getServers(void)
{
	return _servers;
}

void				Webserv::addServerSocket(int port, int nbr_queue)
{
	int			fd_sock;
	int			opt;
	t_poll		pollfd;
	socklen_t	socklen;
	t_sockaddr_in	sockaddr;

	opt = 1;
	if ((fd_sock = socket(AF_INET, SOCK_STREAM, 0)) < 1)
		exit(EXIT_FAILURE);														//TODO: better error handling
	
	// Set NON-BLOCKING and SO_REUSEADDR
	// Avoid EADDRINUSE -> BIND and LISTEN should not return any error
	fcntl(fd_sock, F_SETFL, O_NONBLOCK);
	setsockopt(fd_sock, SOL_SOCKET, SO_REUSEADDR, (char *)(&opt), sizeof(opt));

	bzero(&sockaddr, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddr.sin_port = htons(port);
	bind(fd_sock, (t_sockaddr *) &sockaddr, sizeof(sockaddr));

	nbr_queue = (nbr_queue > SOMAXCONN ? SOMAXCONN : nbr_queue);
	listen(fd_sock, nbr_queue);

	pollfd.fd = fd_sock;
	pollfd.events = POLL_FLAGS;
	pollfd.revents = 0;
	_pollfd.push_back(pollfd);
	++_fdInUse;

	_serverSocks.push_back(NetworkSocket(fd_sock, htonl(INADDR_ANY), _pollfd.back()));
}

void				Webserv::checkServerSockets(void)
{
	int				fd_client;
	in_addr_t		addr;
	socklen_t		socklen;
	short			revents;
	t_sockaddr_in	sockaddr;


	for (it_netSock it = _serverSocks.begin(); it != _serverSocks.end(); ++it)
	{
		// TODO: check _fdInUse < open_max
		// TODO: check other values of revents
		revents = (it->getPollFd()).revents;
		if (revents == POLLOUT)
			continue ;
		if (revents | POLLIN)
		{
			socklen = sizeof(sockaddr);
			if ((fd_client = accept((it->getPollFd()).fd, (t_sockaddr *) &sockaddr, &socklen)) < 0)
				exit(EXIT_FAILURE);												//TODO: better error handling;
			fcntl(fd_client, F_SETFL, O_NONBLOCK);
			addr = sockaddr.sin_addr.s_addr;
			_clientSocks.push_back(ClientSocket(it->getPort(), addr, addPollfd(fd_client), *this));
			if (_fdInUse == open_max)
				break ;
		}
	}
}

/*
** - POLLHUP: peer closed its end of the channel
** - POLLNVAL: fd not open
** - POLLERR: error
*/

void				Webserv::checkClientSockets(void)
{
	ssize_t			n;
	it_cliSock		client;

	client = _clientSocks.begin();
	while (client != _clientSocks.end())
	{
		client->getNewRequest();
		// Handle Request's Stack - 
		// Handle Response's Stack
		// Update DEQUE
		++client;
	}
}

void				Webserv::reapClosedClientSock(void)
{
	for (it_cliSock it = _clientSocks.begin(); it != _clientSocks.end(); ++it)
	{
		if (it->getState() != NetworkSocket::State::OPEN && !it->empty())
		{
			popPollfd(it->getPollFd());
			_clientSocks.erase(it);
		}
	}
}


t_poll&				Webserv::addPollfd(int fd_client)
{
	t_poll							new_pollfd;
	std::vector<t_poll>::iterator	it;

	if (_fdInUse == _pollfd.size())
	{
		new_pollfd.fd = fd_client;
		new_pollfd.events = POLL_FLAGS;
		new_pollfd.revents = 0;
		_pollfd.push_back(new_pollfd);
		++_fdInUse;
		return _pollfd.back();
	}
	else
	{
		it = _pollfd.begin();
		while (it != _pollfd.end())
		{
			if (it->fd < 0)
				break ;
			 ++it;
		}
		it->fd = fd_client;
		it->events = POLL_FLAGS;
		it->revents = 0;
		++_fdInUse;
		return *it;
	}
}

void				Webserv::popPollfd(t_poll&	pollfd)
{
	pollfd.fd = -1;
	--_fdInUse;
}

int					Webserv::isValidRequest(const std::string& method)
{
	for (std::string::size_type i = 0; i < sizeof(_validMethod)/sizeof(std::string); ++i)
	{
		if (ci_equal(method, _validMethod[i]))
			return 1;
	}
	return 0;
}

#endif
