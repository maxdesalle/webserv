/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 15:54:27 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/14 11:59:09 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORKSOCKET_HPP
# define NETWORKSOCKET_HPP

# include <unistd.h>
# include <iostream>

# include "NetworkIPC.hpp"
# include "Timer.hpp"

# define	TIMEOUT			3 * 60 * 1000
# define	TIMEOUT_NVAL	100

class NetworkSocket
{
public:
	enum State {OPEN, HALF_CLOSED, CLOSED};

	int				_port;
	in_addr_t		_addr;
	t_poll&			_pollfd;
	State			_state;
	Timer			_timer;

	NetworkSocket(int port, in_addr_t addr, t_poll& pollfd);
	virtual ~NetworkSocket(void);

	in_addr_t	getIP(void);
	int			getPort(void);
	void		sockShutdown(void);
	void		sockClose(void);
	void		handlePollErr(void);
};

NetworkSocket::NetworkSocket(int port, in_addr_t addr, t_poll& pollfd)
:_port(port), _addr(addr), _pollfd(pollfd), _state(OPEN)
{
	_timer.start();
}

NetworkSocket::~NetworkSocket() {}

in_addr_t	NetworkSocket::getIP(void)
{
	return _addr;
}

int			NetworkSocket::getPort(void)
{
	return _port;
}

void		NetworkSocket::sockShutdown(void)
{
	if (_state == OPEN)
	{
		shutdown(_pollfd.fd, SHUT_WR);
		_state = HALF_CLOSED;
	}
	else
		sockClose();
}

void		NetworkSocket::sockClose(void)
{
	shutdown(_pollfd.fd, SHUT_RDWR);
	close(_pollfd.fd);
	_pollfd.fd = -1;
	_state = CLOSED;
}

void		NetworkSocket::handlePollErr(void)  // TODO: error handling
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

#endif
