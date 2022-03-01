/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:58:04 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/25 09:24:49 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

std::string const Webserv::_validMethod[4] = {"GET", "HEAD", "POST", "DELETE"};

Webserv::Webserv(void) {}

Webserv::~Webserv(void)
{
	_servers.clear();
	_listenSocks.clear();
	_clientSocks.clear();
	delete [] _pollfd;
}

// Webserv&	Webserv::operator=(Webserv const& rhs)
// {
// 	if (this != &rhs)
// 	{
// 		_fdInUse = rhs._fdInUse;
// 		_servers = rhs._servers;
// 		_pollfd = rhs._pollfd;
// 		_listenSocks = rhs._listenSocks;
// 		_clientSocks = rhs._clientSocks;
// 	}
// 	return *this;
// }

vecServer&			Webserv::getServers(void)
{
	return _servers;
}

void				Webserv::initWebserv(std::string const& config)
{
	_setOpenMax();
	_pollfd = new t_poll[open_max];
	_fdInUse = 0;
	_nbrPollMax = 0;
	std::cout	<< "\e[32m \t############################# \n" \
				<< " \t#          PARSING          # \n" \
				<< " \t############################# \e[0m" << std::endl;
	_servers = ConfigHandler(config);
	printServers(_servers);
	std::cout	<< "\e[32m \t#############################\n" \
				<< " \t#          WEBSERV          #\n" \
				<< " \t############################# \e[0m" << std::endl;
	_setNetworkSockets();
	std::cout	<< " NetworkSockets: \e[31m" << _listenSocks.size() << "\e[0m\n" \
				<< "       _fdInUse: \e[31m" << _fdInUse << "\e[0m" << std::endl;
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

void				Webserv::runWebserv(void)
{
	while (true)
	{
		std::cout << "\n[Listen: " << _nbrPorts << "; Client: " << _fdInUse - _nbrPorts << "; PollMax:" << _nbrPollMax << "]"<< std::endl;
		std::cout << "\e[34m ####################################\n" \
					<< " #   Poll, Waiting For An Revents   #\n" \
					<< " #################################### \e[0m" << std::endl;
		
		_nbrPoll = poll(_pollfd, _nbrPollMax + 1, -1);
		
		std::cout << "[Poll: \e[0m" << _nbrPoll <<  ": Listen " << __getNbrPollNetwork() \
			<< ", Client " << __getNbrPollClient() << "]" << std::endl;
		
		std::cout << "\e[33m###################   Listen Sockets\e[0m" << std::endl;
		_checkServerSockets();
		std::cout << "\e[33m###################   Client Sockets\e[0m" << std::endl;
		if (__getNbrPollClient())
			_checkClientSockets();
	}
}

void				Webserv::_checkServerSockets(void)
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
			_clientSocks.push_back(ClientSocket(it->getPort(), addr, addPollfd(fd_client), *this));
			std::cout << _clientSocks.back() << std::endl; // TODO: DEBUG
			if (_fdInUse == open_max)
				break ;
		}
	}
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

void				Webserv::_checkClientSockets(void)
{
	itClieSock		client;

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
	for (itClieSock it = _clientSocks.begin(); it != _clientSocks.end(); ++it)
	{
		if (!it->isOpen() && !it->empty())
		{
			popPollfd(it->getPollFd());
			_clientSocks.erase(it);
		}
	}
}

t_poll&				Webserv::addPollfd(int fd_client)
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

void				Webserv::popPollfd(t_poll&	pollfd)
{
	pollfd.fd = -1;
	while (_pollfd[_nbrPollMax].fd == -1)
		--_nbrPollMax;
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
