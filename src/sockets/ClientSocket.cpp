/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:55:52 by tderwedu          #+#    #+#             */
/*   Updated: 2022/03/02 13:18:57 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSocket.hpp"


ClientSocket::ClientSocket(int port, in_addr_t addr, t_poll& pollfd, Webserv& webserv) : ListenSocket(port, addr, pollfd), _webserv(webserv)
{
	_timer.start();
	_messages.push_back(RequestHandler());
}

ClientSocket::ClientSocket(ClientSocket const& rhs) : ListenSocket(rhs._pollfd), _webserv(rhs._webserv) { *this = rhs; }

ClientSocket::~ClientSocket() {}

ClientSocket&	ClientSocket::operator=(ClientSocket const& rhs)
{
	if (this != &rhs)
	{
		// std::cout << "\e[31mHELLO FROM ClientSocket::operator=\e[0m" << std::endl; // TODO:remove
		_port = rhs._port;
		_addr = rhs._addr;
		_pollfd = rhs._pollfd;
		_state = rhs._state;
		_messages = rhs._messages;
		_timer = rhs._timer;
	}
	return *this;
}

void		ClientSocket::getNewRequest(void)
{
	int				ret;
	ssize_t			n;
	RequestHandler&	handler = _messages.back();
	Request&		request = handler.getRequest();
	char			_buff[RECV_BUFF_SIZE + 1];
	std::string		buff;

	std::cout << "\e[32m ===> New Request\e[0m" << std::endl;
	std::cout << "     -   Port: " << _port << std::endl;
	std::cout << "     - PollFD: " << _pollfd.fd << " ; " << _pollfd.revents << std::endl;

	if (_pollfd.fd < 0)
	{
		std::cout << " \e[31m \t \t _pollfd.fd < 0 ! \e[0m" << std::endl; // TODO:remove
		return ;
	}
	if (_pollfd.revents & (POLLHUP | POLLERR)) // Client disconnected or Any error
	{
		std::cout << " \e[31m \t \t POLLHUP | POLLERR ! \e[0m" << std::endl; // TODO:remove
		_clearSocket();
	}
	else if (_pollfd.revents & POLLNVAL) // FD not open
	{
		std::cout << " \e[31m \t \t TIMEOUT ! \e[0m" << std::endl; // TODO:remove
		if (_timer.getElapsedTime() < TIMEOUT_NVAL)
			return ;
		_pollfd.fd = -1;
		_state = CLOSED;
	}
	else if (_pollfd.revents & POLLIN)
	{
		n = recv(_pollfd.fd, _buff, RECV_BUFF_SIZE, 0);
		_buff[n] = '\0';
		buff = std::string(_buff);
		// std::cout << "BUFF:\e[31m>>\e[0m" << _buff << "\e[31m<<\e[0m" << std::endl;
		if (n < 0)
			_clearSocket();
		if (n == 0) // Client initated a gracefull close
		{
			std::cout << " \e[31m \t \t Connection CLosed ! \e[0m" << std::endl; // TODO:remove
			request.reset();
			sockClose();
			return ;
		}
		if (_state == HALF_CLOSED)
			return ;
		ret = request.parseRequest(buff);
		if (ret > 1) // TODO: add error handling
			std::cout << "Error parseRequest: " << ret << std::endl;
		if (request.isProcessing())
		{
			std::cout	<< "\e[32m"	<< " \t################### \n" \
									<< " \t#     Request     # \n" \
									<< " \t################### \e[0m" << std::endl;
			std::cout	<< request << std::endl;
			std::cout	<< " \t\e[32m------------------- \e[0m\n" << "";
			if (_webserv.isValidRequest(request.getMethod()))
				; // TODO: reponse "501 Not Implemented"
			_findServer();
			_messages.push_back(RequestHandler());
		}	
	}
	else if (!request.isProcessing() && _timer.getElapsedTime() > TIMEOUT) // TODO: check first getElapsedTime and reset timer a each POLLIN
	{
		std::cout << " \e[31m \t \t TIMEOUT while processing ! \e[0m" << std::endl; // TODO:remove
		_clearSocket();
	}
}

void		ClientSocket::sendResponse(void)
{
	ssize_t			n;
	RequestHandler&	handler = _messages.front();
	Response&		response = handler.getResponse();
	Request&		request = handler.getRequest();

	handler._findLocation(request.getTarget());
	std::string const& buff = response.GetHeaderResponse(request, const_cast<Location&>(handler.getLocation())); //TODO: THIS IS UGLY!!!!
	n = send(_pollfd.fd, buff.c_str(), buff.size(), 0);
	if (n < 0)
	{
		std::cout << " \e[31m \t \t TIMEOUT while processing ! \e[0m" << std::endl; // TODO:remove
		_clearSocket();
	}
}

int			ClientSocket::empty(void)
{
	return (_messages.size() <= 1);
}

void		ClientSocket::_findServer(void)										// TODO: CORR matchingServers
{
	struct in_addr				addr;
	char						ip[INET_ADDRSTRLEN];
	std::vector<Server const*>	*matchingServers;
	RequestHandler&				handler = _messages.back();
	Request&					request = handler.getRequest();
	std::string const&			host = request.getField("Host");

	// std::cout << "======================> _findServer" << std::endl; //TODO: remove
	addr.s_addr = _addr;
	inet_ntop(AF_INET, &addr, ip, INET_ADDRSTRLEN);
	matchingServers = FindMatchingServers(_webserv.getServers(), _port, ip);
	if (matchingServers->empty())
		; // TODO: reponse "500 Internal Server Error"
	if (host.empty())															//TODO: Missing HOST field might be an error!
		handler.setServer(*matchingServers->at(0));
	for (size_t i = 0; i < matchingServers->size(); ++i)
	{
		for (size_t j = 0; j < (matchingServers->at(i))->GetServerNames().size(); ++j)
		{
			if (host == (matchingServers->at(i))->GetServerNames()[j])
			{
				handler.setServer(*matchingServers->at(i));
				delete matchingServers;
				return ;
			}
		}
	}
	handler.setServer(*matchingServers->at(0));
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

std::ostream&			operator<<(std::ostream& stream, ClientSocket const& sock)
{
	stream << " \e[32m===> ClientSocket \e[0m" << std::endl;
	stream << "     -   Port: " << sock._port << std::endl;
	stream << "     - PollFD: " << sock._pollfd.fd << " ; " \
			<< sock._pollfd.events << " ; " << sock._pollfd.revents;
	return stream;
}
