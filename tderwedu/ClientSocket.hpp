/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 15:22:19 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/03 15:56:45 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include "NetworkIPC.hpp"
# include "NetworkSocket.hpp"
# include "Request.hpp"

# define	BUFF_SIZE	1024

class ClientSocket : public NetworkSocket
{
public:
	static const int	buffSize = BUFF_SIZE;
private:
	Request				_request;
	std::string			_msg;
	char				_buff[BUFF_SIZE];

public:
	ClientSocket(int port, in_addr_t addr, t_poll& pollfd);
	virtual ~ClientSocket();

	void		getRequest(void);
};

ClientSocket::ClientSocket(int port, in_addr_t addr, t_poll& pollfd)
: NetworkSocket(port, addr, pollfd)
{}

ClientSocket::~ClientSocket()
{}

void		ClientSocket::getRequest(void)
{
	ssize_t			n;

	if (_pollfd.fd < 0)
		return ;
	if (_pollfd.revents | (POLLHUP & POLLERR)) // Client disconnected
	{
		// If a request is being processed
		_request.stopRequest();
		sockClose();
	}
	else if (_pollfd.revents | POLLNVAL) // FD not open
	{
		if (_timer.getElapsedTime() < TIMEOUT_NVAL)
			return ;
		_pollfd.fd = -1;
		_state = CLOSED;
		// exit(EXIT_FAILURE);
	}
	else if (_pollfd.revents | POLLERR)
	{
		_request.stopRequest();
		sockClose();
		// exit(EXIT_FAILURE);
	}
	else if (_pollfd.revents | POLLIN)
	{
		n = recv(_pollfd.fd, _buff, buffSize, 0);
		if (n <= 0)
		{
			// If a request is being processed
			_request.stopRequest();
			sockClose();
		}
		if (_state == HALF_CLOSED)
			return ;
		_msg.append(_buff);
		_msg = _request.parseRequest(_msg);
		if (_request.getState() == Request::State::DOWNLOADED)
			;//TODO: find server and add request to the resquest's stack
	}
	else
	{
		if (_request.getState() < Request::State::PROCESSING && _timer.getElapsedTime() > TIMEOUT)
		{
			_request.stopRequest();
			sockClose();
		}
	}
}
#endif
