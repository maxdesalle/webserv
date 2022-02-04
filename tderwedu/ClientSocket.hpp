/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 15:22:19 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/04 17:45:57 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <deque>

# include "Config.hpp"

# include "NetworkIPC.hpp"
# include "NetworkSocket.hpp"

# include "Request.hpp"
# include "Response.hpp"

# define	BUFF_SIZE	1024

class ClientSocket : public NetworkSocket
{
public:
	static const int	buffSize = BUFF_SIZE;
private:
	std::deque<Request>		_requests;
	std::deque<Response>	_responses;
	std::string				_msg;
	char					_buff[BUFF_SIZE];

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
	ssize_t		n;
	Request&	request = _requests.back();

	if (_pollfd.fd < 0)
		return ;
	if (_pollfd.revents | (POLLHUP & POLLERR)) // Client disconnected
	{
		request.stopRequest(); // If a request is being processed
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
		request.stopRequest(); // If a request is being processed
		sockClose();
		// exit(EXIT_FAILURE);
	}
	else if (_pollfd.revents | POLLIN)
	{
		n = recv(_pollfd.fd, _buff, buffSize, 0);
		if (n <= 0)
		{
			request.stopRequest(); // If a request is being processed
			sockClose();
		}
		if (_state == HALF_CLOSED)
			return ;
		_msg.append(_buff);
		_msg = request.parseRequest(_msg);
		if (request.getState() == Request::State::DOWNLOADED)
		{
			findServer(request);
			_requests.push_back(Request());
		}	
	}
	else
	{
		if (request.getState() < Request::State::PROCESSING && _timer.getElapsedTime() > TIMEOUT)
		{
			request.stopRequest(); // If a request is being processed
			sockClose();
		}
	}
}

#endif
