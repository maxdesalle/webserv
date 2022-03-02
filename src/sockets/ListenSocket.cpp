/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 16:13:15 by tderwedu          #+#    #+#             */
/*   Updated: 2022/03/02 21:44:31 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ListenSocket.hpp"

/*
** ListenSocket
** ================================ Constructors ===============================
*/

ListenSocket::ListenSocket(t_poll& pollfd): _pollfd(pollfd) {}
ListenSocket::ListenSocket(int port, in_addr_t addr, t_poll& pollfd)
: _pollfd(pollfd), _port(port), _addr(addr), _sockState(OPEN)
{}

ListenSocket::ListenSocket(ListenSocket const& rhs) : _pollfd(rhs._pollfd) { *this = rhs; }
ListenSocket::~ListenSocket() {}

ListenSocket&	ListenSocket::operator=(ListenSocket const& rhs)
{
	if (this != &rhs)
	{
		_port = rhs._port;
		_addr = rhs._addr;
		_pollfd = rhs._pollfd;
		_sockState = rhs._sockState;
	}
	return *this;
}

/*
** ListenSocket
** ============================== Member functions =============================
*/

in_addr_t					ListenSocket::getIP(void)
{
	return _addr;
}

int							ListenSocket::getPort(void)
{
	return _port;
}

t_poll&						ListenSocket::getPollFd(void)
{
	return _pollfd;
}

void						ListenSocket::sockShutdown(void)
{
	if (_sockState == OPEN)
	{
		shutdown(_pollfd.fd, SHUT_WR);
		_sockState = HALF_CLOSED;
	}
	else
		sockClose();
}

void						ListenSocket::sockClose(void)
{
	shutdown(_pollfd.fd, SHUT_RDWR);
	close(_pollfd.fd);
	_pollfd.fd = -1;
	_sockState = CLOSED;
}

/*
** ListenSocket
** ============================ Non-Member functions ===========================
*/

std::ostream&				operator<<(std::ostream& stream, ListenSocket const& sock)
{
	stream << " \e[36m===> ListenSocket \e[0m" << std::endl;
	stream << "     -   Port: " << sock._port << std::endl;
	stream << "     - PollFD: " << sock._pollfd.fd << " ; " \
			<< sock._pollfd.events << " ; " << sock._pollfd.revents;
	return stream;
}
