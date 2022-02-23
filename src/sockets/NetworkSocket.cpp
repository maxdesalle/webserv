/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkSocket.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:56:44 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/23 17:37:06 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NetworkSocket.hpp"

NetworkSocket::NetworkSocket(int port, in_addr_t addr, t_poll& pollfd)
:_port(port), _addr(addr), _pollfd(pollfd), _state(OPEN)
{}

NetworkSocket::~NetworkSocket() {}

NetworkSocket&	NetworkSocket::operator=(NetworkSocket const& rhs)
{
	if (this != &rhs)
	{
		_port = rhs._port;
		_addr = rhs._addr;
		_pollfd = rhs._pollfd;
		_state = rhs._state;
	}
	return *this;
}

in_addr_t				NetworkSocket::getIP(void)
{
	return _addr;
}

int						NetworkSocket::getPort(void)
{
	return _port;
}

t_poll&					NetworkSocket::getPollFd(void)
{
	return _pollfd;
}

NetworkSocket::State	NetworkSocket::getState(void)
{
	return _state;
}

bool		NetworkSocket::isOpen(void) const
{
	return _state == OPEN;
}

void					NetworkSocket::sockShutdown(void)
{
	if (_state == OPEN)
	{
		shutdown(_pollfd.fd, SHUT_WR);
		_state = HALF_CLOSED;
	}
	else
		sockClose();
}

void					NetworkSocket::sockClose(void)
{
	shutdown(_pollfd.fd, SHUT_RDWR);
	close(_pollfd.fd);
	_pollfd.fd = -1;
	_state = CLOSED;
}

void					NetworkSocket::handlePollErr(void)  // TODO: error handling
{
	int			err;
	socklen_t	size;

	size = sizeof(err);
	std::cout << "Error with FD: " << _pollfd.fd << " and port " << _port << std::endl;
	if (getsockopt(_pollfd.fd, SOL_SOCKET, SO_ERROR, &err, &size) < 0)
		std::cout << "Error while getting socket error!" << std::endl;
	else
		std::cout << "SO_ERROR: " << err << std::endl;
	sockClose();
	// exit(EXIT_FAILURE);
}
