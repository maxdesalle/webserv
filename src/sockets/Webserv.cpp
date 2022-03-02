/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:58:04 by tderwedu          #+#    #+#             */
/*   Updated: 2022/03/02 18:53:56 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

inline static void	__debug_header__(char const *header)	// TODO: DEBUG
{
	std::cout	<< "\e[32m \t############################# \n" \
				<< " \t#           " << header << "          # \n" \
				<< " \t############################# \e[0m" << std::endl;
}

inline static void	__debug_section__(char const *section)
{
	std::cout << "\e[33m###################   "<< section << "\e[0m" << std::endl;
}

inline static void	__debug_poll__(void)	// TODO: DEBUG
{
		std::cout << "\e[34m ####################################\n" \
					<< " #   Poll, Waiting For An Revents   #\n" \
					<< " #################################### \e[0m" << std::endl;
}

inline void		Webserv::__debug_before_poll__(void) const	// TODO: DEBUG
{
	std::cout	<< "\n[Listen: " << _nbrPorts << "; Client: " \
				<< _fdInUse - _nbrPorts << "; PollMax:" << _nbrPollMax\
				<< "]"<< std::endl;
}

inline void		Webserv::__debug_after_poll__(void) const	// TODO: DEBUG
{
	std::cout	<< "[Poll: \e[0m" << _nbrPoll <<  ": Listen " \
				<< __getNbrPollNetwork() << ", Client " \
				<< __getNbrPollClient() << "]" << std::endl;
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
	__debug_header__("PARSING");
	printServers(_servers);
	__debug_header__("WEBSERV");
	_setNetworkSockets();
}

void				Webserv::runWebserv(void)
{
	while (true)
	{
		__debug_before_poll__();
		__debug_poll__();
		_nbrPoll = poll(_pollfd, _nbrPollMax + 1, -1);
		__debug_after_poll__();
		__debug_section__("Listen Sockets");
		_checkListenSockets(); //TODO: change
		__debug_section__("Client Sockets");
		_checkClientSockets();
		sleep(1); // TODO: remove
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
				std::cerr << "sysconf error for _SC_OPEN_MAX" << std::endl;		//TODO: better error handling
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
	std::cout << " Listening Sockets: \e[31m" << _nbrPorts << "\e[0m" << std::endl; // TODO: DEBUG
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

	_pollfd[_nbrPollMax].fd = fd_sock;
	_pollfd[_nbrPollMax].events = POLL_FLAGS;
	_pollfd[_nbrPollMax].revents = 0;
	_listenSocks.push_back(ListenSocket(port, htonl(INADDR_ANY), _pollfd[_nbrPollMax]));
	++_nbrPollMax;
	++_fdInUse;
	std::cout << _listenSocks.back() << std::endl; // TODO: DEBUG
}

void				Webserv::_checkListenSockets(void)
{
	int				fd_client;
	in_addr_t		addr;
	socklen_t		socklen;
	short			revents;
	t_sockaddr_in	sockaddr;

	int i = -1;
	for (itlistSock it = _listenSocks.begin(); it != _listenSocks.end(); ++it)
	{
		++i;
		// TODO: check _fdInUse < open_max
		// TODO: check other values of revents
		revents = (it->getPollFd()).revents;
		if (revents == POLLOUT)
			continue ;
		if (revents & POLLIN)
		{
			std::cout << "\e[35m ===> New connection from PORT: \e[0m" << it->getPort() << std::endl;
			socklen = sizeof(sockaddr);
			if ((fd_client = accept((it->getPollFd()).fd, (t_sockaddr *) &sockaddr, &socklen)) < 0)
			{
				std::cout << "\e[31m ERROR: accept" << std::endl;
				exit(EXIT_FAILURE);												//TODO: better error handling;
			}
			fcntl(fd_client, F_SETFL, O_NONBLOCK);
			addr = sockaddr.sin_addr.s_addr;
			_clientSocks.push_back(ClientSocket(it->getPort(), addr, _pushPollfd(fd_client), *this));
			std::cout << _clientSocks.back() << std::endl; // TODO: DEBUG
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
		if (client->handleSocket())
		{
			_popPollfd(client->getPollFd());
			_clientSocks.erase(client);
		}
		++client;
	}
}

t_poll&				Webserv::_pushPollfd(int fd_client)
{
	size_t	i = 0;

	if (_nbrPollMax == _fdInUse)
	{
		++_nbrPollMax;
		i = _nbrPollMax;
	}
	else
	{
		while (_pollfd[i].fd != -1)
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
	while (_pollfd[_nbrPollMax].fd == -1)
		--_nbrPollMax;
	--_fdInUse;
}

size_t				Webserv::__getNbrPollNetwork(void) const
{
	size_t	nbr = 0;

	for (size_t i = 0; i < _nbrPorts; ++i)
		nbr += (_pollfd[i].revents & POLLIN);
	return nbr;
}

size_t				Webserv::__getNbrPollClient(void) const
{
	size_t	nbr = 0;

	for (size_t i = _nbrPorts; i <= _nbrPollMax; ++i)
		nbr += (_pollfd[i].revents & POLLIN);
	return nbr;
}
