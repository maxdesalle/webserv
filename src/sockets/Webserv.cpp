/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:58:04 by tderwedu          #+#    #+#             */
/*   Updated: 2022/03/15 15:40:26 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

inline static void		___error_msg___(char const *fct, char const *msg)
{
	std::cerr << "\e[31m error \e[0m" << fct << ": " << msg << std::endl; 
}

inline static void		___error_msg___(char const *fct, char const *msg, int port)
{
	std::cerr	<< "\e[31m error \e[0m" << fct << ": " << msg << "\e[31m" \
				<< port << "\e[0m" << std::endl; 
}


inline static void	___debug_header___(char const *header)
{
#ifdef DEBUG
	std::cout	<< "\e[32m \t############################# \n" \
				<< " \t#           " << header << "          # \n" \
				<< " \t############################# \e[0m" << std::endl;
#else
	(void)header;
#endif
}

inline static void	___debug_section___(char const *section)
{
#ifdef DEBUG2
		std::cout << "\e[33m###################   "<< section << "\e[0m" << std::endl;
#else
	(void)section;
#endif
}

inline static void	___debug_poll___(void)
{
#ifdef DEBUG2
		std::cout << "\e[34m ####################################\n" \
					<< " #   Poll, Waiting For An Revents   #\n" \
					<< " #################################### \e[0m" << std::endl;
#endif
}

inline void		Webserv::___debug_before_poll___(void) const
{
#ifdef DEBUG2
		std::cout	<< "\n[Listen: " << _nbrPorts << "; Client: " \
					<< _fdInUse - _nbrPorts << "; _fdInUse:" << _fdInUse\
					<< "; PollMax:" << _nbrPollMax << "]"<< std::endl;
#endif
}

inline void		Webserv::___debug_after_poll___(void) const
{
#ifdef DEBUG2
	size_t	nbr_l = __getNbrPollClient();
	size_t	nbr_c = __getNbrPollListen();

	std::cout	<< "[Poll: \e[0m" << _nbrPoll <<  ": Listen " \
				<< nbr_l << ", Client " \
				<< nbr_c << "]" << std::endl;
#endif
}

/*
** Webserv
** ================================ Constructors ===============================
*/

Webserv::Webserv(void) {}

Webserv::~Webserv(void)
{
	_servers.clear();
	_listenSocks.clear();
	_clientSocks.clear();
	delete [] _pollfd;
}

/*
** Webserv
** ============================== Member functions =============================
*/

vecServer const&	Webserv::getServers(void) const
{
	return _servers;
}

void				Webserv::initWebserv(std::string const& config)
{
	_setOpenMax();
	_pollfd = new t_poll[open_max];
	_fdInUse = 0;
	_nbrPollMax = 0;
	_servers = ConfigHandler(config);
	___debug_header___("PARSING");
#ifdef DEBUG
	printServers(_servers);
#endif
	___debug_header___("WEBSERV");
	_setNetworkSockets();
}

void				Webserv::runWebserv(void)
{
	std::cout << "\e[0m\t is running\n" << std::endl;
	while (true)
	{
		___debug_before_poll___();
		___debug_poll___();
		_nbrPoll = poll(_pollfd, _nbrPollMax + 1, 0);
		___debug_after_poll___();
		___debug_section___("Listen Sockets");
		_checkListenSockets();
		___debug_section___("Client Sockets");
		_checkClientSockets();
#ifdef DEBUG2
		sleep(1);
#endif
	}
}

void				Webserv::_setOpenMax(void) const
{
	if (!open_max)
	{
		errno = 0;
		if ((open_max = sysconf(_SC_OPEN_MAX)) < 0)
		{
			if (errno == 0)
				open_max = OPEN_MAX_GUESS;
			else
			{
				___error_msg___("sysconf", "error for _SC_OPEN_MAX");
				exit(EXIT_FAILURE);
			}
		}
	}
}

void				Webserv::_setNetworkSockets(void)
{
	std::set<int>	ports;
	mapServer		ip_ports;

	for (citServer serv = _servers.begin(); serv != _servers.end(); ++serv)
	{
		ip_ports = serv->GetListenIPandPorts();
		for (mapServer::const_iterator cit = ip_ports.begin(); cit != ip_ports.end(); ++cit)
		{
			for (std::vector<size_t>::const_iterator port = cit->second.begin(); port != cit->second.end(); ++port)
				ports.insert(*port);
		}
	}
	_nbrPorts = ports.size();
#ifdef DEBUG2
	std::cout << " Listening Sockets: \e[31m" << _nbrPorts << "\e[0m" << std::endl;
#endif
	for (std::set<int>::const_iterator port = ports.begin(); port != ports.end(); ++port)
		_addNetworkSocket(*port, SOMAXCONN);
}

void				Webserv::_addNetworkSocket(int port, int nbr_queue)
{
	int				fd_sock;
	int				opt;
	t_sockaddr_in	sockaddr;

	opt = 1;
	if ((fd_sock = socket(AF_INET, SOCK_STREAM, 0)) < 1)
	{
		___error_msg___("socket", strerror(errno));
		exit(EXIT_FAILURE);
	}
	// Set NON-BLOCKING and SO_REUSEADDR
	// Avoid EADDRINUSE -> BIND and LISTEN should not return any error
	fcntl(fd_sock, F_SETFL, O_NONBLOCK);
	setsockopt(fd_sock, SOL_SOCKET, SO_REUSEADDR, (char *)(&opt), sizeof(opt));

	bzero(&sockaddr, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockaddr.sin_port = htons(port);
	errno = 0;
	if (bind(fd_sock, (t_sockaddr *) &sockaddr, sizeof(sockaddr)))
	{
		if (errno == EACCES)
			___error_msg___("bin", "need superuser privileges for port ", port);
		else
			___error_msg___("bin", strerror(errno));
		exit(EXIT_FAILURE);
	}
	nbr_queue = (nbr_queue > SOMAXCONN ? SOMAXCONN : nbr_queue);
	if (listen(fd_sock, nbr_queue))
	{
		___error_msg___("listen", strerror(errno));
		exit(EXIT_FAILURE);
	}
	_pollfd[_nbrPollMax].fd = fd_sock;
	_pollfd[_nbrPollMax].events = POLL_FLAGS;
	_pollfd[_nbrPollMax].revents = 0;
	_listenSocks.push_back(ListenSocket(port, htonl(INADDR_ANY), _pollfd[_nbrPollMax]));
	++_nbrPollMax;
	++_fdInUse;
#ifdef DEBUG2
	std::cout << _listenSocks.back() << std::endl;
#endif
}

void				Webserv::_checkListenSockets(void)
{
	int				fd_client;
	in_addr_t		addr;
	socklen_t		socklen;
	short			revents;
	t_sockaddr_in	sockaddr;

	for (itlistSock it = _listenSocks.begin(); it != _listenSocks.end(); ++it)
	{
		revents = (it->getPollFd()).revents;
		if (revents == POLLOUT)
			continue ;
		if (revents & (POLLHUP | POLLERR ))
		{
			___error_msg___("", "an unexpected error occured for a listening socket for port ", it->getPort());
			exit(EXIT_FAILURE);
		}
		else if (revents & POLLIN)
		{
#ifdef DEBUG2
			std::cout << "\e[35m ===> New connection from PORT: \e[0m" << it->getPort() << std::endl;
#endif
			socklen = sizeof(sockaddr);
			if ((fd_client = accept((it->getPollFd()).fd, (t_sockaddr *) &sockaddr, &socklen)) < 0)
			{
				___error_msg___("accept", "could not accept a mew connection from port", (it->getPollFd()).fd);
				exit(EXIT_FAILURE);
			}
			fcntl(fd_client, F_SETFL, O_NONBLOCK);
			addr = sockaddr.sin_addr.s_addr;
			_clientSocks.push_back(ClientSocket(it->getPort(), addr, _pushPollfd(fd_client), *this));
#ifdef DEBUG2
			std::cout << _clientSocks.back() << std::endl;
#endif
			if (_fdInUse == open_max)
				break ;
		}
	}
}

void				Webserv::_checkClientSockets(void)
{
	itClieSock		client;

	client = _clientSocks.begin();
	while (client != _clientSocks.end())
	{
#ifdef DEBUG2
		std::cout << *client << std::endl;
#endif
		if (client->handleSocket(_fdInUse == open_max))
		{
			_popPollfd(client->getPollFd());
			client = _clientSocks.erase(client);
		}
		else
			++client;
	}
}

t_poll&				Webserv::_pushPollfd(int fd_client)
{
	size_t	i = 0;

	if (_nbrPollMax == _fdInUse)
		i = _nbrPollMax++;
	else
	{
		while (_pollfd[i].fd > 0)
			++i;
	}
	_pollfd[i].fd = fd_client;
	_pollfd[i].events = POLL_FLAGS;
	_pollfd[i].revents = 0;
	++_fdInUse;
	return _pollfd[i];
}

void				Webserv::_popPollfd(t_poll&	pollfd)
{
	pollfd.fd = -1;
	--_fdInUse;
	while (_pollfd[_nbrPollMax - 1].fd < 1)
	{
		_pollfd[--_nbrPollMax].fd = 0;
	}
}

size_t				Webserv::__getNbrPollListen(void) const
{
	size_t	nbr = 0;

	for (size_t i = 0; i < _nbrPorts; ++i)
		nbr += ((_pollfd[i].revents & POLLIN) != 0);
	return nbr;
}

size_t				Webserv::__getNbrPollClient(void) const
{
	size_t	nbr = 0;

	for (size_t i = _nbrPorts; i < _nbrPollMax; ++i)
		nbr += (_pollfd[i].fd > 0 && _pollfd[i].revents & POLLIN);
	return nbr;
}
