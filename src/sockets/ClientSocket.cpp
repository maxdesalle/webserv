/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:55:52 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/23 17:50:57 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSocket.hpp"


ClientSocket::ClientSocket(int port, in_addr_t addr, t_poll& pollfd, Webserv& webserv) : NetworkSocket(port, addr, pollfd), _webserv(webserv)
{
	_timer.start();
}

ClientSocket::~ClientSocket()
{}

ClientSocket&	ClientSocket::operator=(ClientSocket const& rhs)
{
	if (this != &rhs)
	{
		_messages = rhs._messages;
		memcpy(_buff, rhs._buff, BUFF_SIZE);
		_timer = rhs._timer;
	}
	return *this;
}

void		ClientSocket::getNewRequest(void)
{
	ssize_t			n;
	RequestHandler&	handler = _messages.back();
	Request&		request = handler.getRequest();

	if (_pollfd.fd < 0)
		return ;
	if (_pollfd.revents & (POLLHUP | POLLERR)) // Client disconnected or Any error
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
		if (n < 0)
			_clearSocket();
		if (n == 0) // Client initated a gracefull close
		{
			request.reset();
			if (_messages.size() == 1) // Not processing any request
				sockShutdown();
		}
		if (_state == HALF_CLOSED)
			return ;
		request.parseRequest(std::string(_buff));
		if (request.isProcessing())
		{
			if (_webserv.isValidRequest(request.getMethod()))
				; // TODO: reponse "501 Not Implemented"
			_findServer();
			_messages.push_back(RequestHandler());
		}	
	}
	else if (!request.isProcessing() && _timer.getElapsedTime() > TIMEOUT)
		_clearSocket();
}

int			ClientSocket::empty(void)
{
	return (_messages.size() <= 1);
}

void		ClientSocket::_findServer(void)										// TODO: CORR matchingServers
{
	struct in_addr			addr;
	char					ip[INET_ADDRSTRLEN];
	vecServer 			*matchingServers;
	RequestHandler&			handler = _messages.back();
	Request&				request = handler.getRequest();
	std::string const&		host = request.getField("Host");

	addr.s_addr = _addr;
	inet_ntop(AF_INET, &addr, ip, INET_ADDRSTRLEN);
	matchingServers = FindMatchingServers(_webserv.getServers(), _port, ip);
	if (matchingServers->empty())
		; // TODO: reponse "500 Internal Server Error"
	if (host.empty())															//TODO: Missing HOST field might be an error!
		handler.setServer(matchingServers->at(0));
	for (size_t i = 0; i < matchingServers->size(); ++i)
	{
		for (size_t j = 0; j < (matchingServers->at(i)).GetServerNames().size(); ++j)
		{
			if (host == (matchingServers->at(i)).GetServerNames()[j])
			{
				handler.setServer(matchingServers->at(i));
				return ;
			}
		}
	}
	handler.setServer(matchingServers->at(0));
	delete matchingServers;
}

void		ClientSocket::_clearSocket(void)
{
	for (itReqHand it = _messages.begin(); it < _messages.end(); ++it)
	{
		it->clearRequestHandler();
	}
	sockClose();
}
