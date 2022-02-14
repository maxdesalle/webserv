/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 15:22:19 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/14 17:53:25 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <deque>

# include "Config.hpp"

# include "NetworkIPC.hpp"
# include "Webserv.hpp"
# include "NetworkSocket.hpp"

# include "RequestHandler.hpp"
# include "../utils/Timer.hpp"

# include "../HTTP_Config/Request.hpp"

# define	BUFF_SIZE	1024

class ClientSocket : public NetworkSocket
{
public:
	static const int	buffSize = BUFF_SIZE;
private:
	Webserv&					_webserv;
	std::deque<RequestHandler>	_messages;
	char						_buff[BUFF_SIZE];
	Timer						_timer;
	

public:
	ClientSocket(int port, in_addr_t addr, t_poll& pollfd, Webserv& webserv);
	virtual ~ClientSocket();

	void		getNewRequest(void);
private:
	void		_clearSocket(void);
	void		_findServer(void);
};

ClientSocket::ClientSocket(int port, in_addr_t addr, t_poll& pollfd, Webserv& webserv) : NetworkSocket(port, addr, pollfd), _webserv(webserv)
{
	_timer.start();
}

ClientSocket::~ClientSocket()
{}

void		ClientSocket::getNewRequest(void)
{
	ssize_t			n;
	RequestHandler&	handler = _messages.back();
	Request&		request = handler.getRequest();

	if (_pollfd.fd < 0)
		return ;
	if (_pollfd.revents & (POLLHUP | POLLERR)) // Client disconnected
		_clearSocket();
	else if (_pollfd.revents & POLLNVAL) // FD not open
	{
		if (_timer.getElapsedTime() < TIMEOUT_NVAL)
			return ;
		_pollfd.fd = -1;
		_state = CLOSED;
	}
	else if (_pollfd.revents & POLLIN)
	{
		n = recv(_pollfd.fd, _buff, buffSize, 0);
		if (n <= 0)
			_clearSocket();
		if (_state == HALF_CLOSED)
			return ;
		request.parseRequest(std::string(_buff));
		if (request.getState() == Request::state::PROCESSING)
		{
			if (_webserv.isValidRequest(request.getMethod()))
				; // TODO: reponse "501 Not Implemented"
			_findServer();
			// 
		}	
	}
	else if (request.getState() < Request::state::PROCESSING && _timer.getElapsedTime() > TIMEOUT)
		_clearSocket();
}

void				ClientSocket::_findServer(void)
{
	struct in_addr			addr;
	char					ip[INET_ADDRSTRLEN];
	std::vector<Server>		matchingServers;
	RequestHandler&			handler = _messages.back();
	Request&				request = handler.getRequest();
	std::string const&		host = request.getField("Host");

	addr.s_addr = _addr;
	inet_ntop(AF_INET, &addr, ip, INET_ADDRSTRLEN);
	matchingServers = FindMatchingServers(_webserv.getServers(), _port, ip);
	if (matchingServers.empty())
		; // TODO: reponse "500 Internal Server Error"
	if (host.empty())															//TODO: Missing HOST field might be an error!
		handler.setServer(matchingServers[0]);
	for (int i; i < matchingServers.size(); ++i)
	{
		for (int j; j < matchingServers[i].GetServerNames().size(); ++j)
		{
			if (host == matchingServers[i].GetServerNames()[j])
			{
				handler.setServer(matchingServers[j]);
				return ;
			}
		}
	}
	handler.setServer(matchingServers[0]);
}


#endif
