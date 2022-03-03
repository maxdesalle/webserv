/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:55:52 by tderwedu          #+#    #+#             */
/*   Updated: 2022/03/03 12:18:59 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSocket.hpp"

inline static void		___debug_msg___(char const *msg) // TODO: DEBUG
{
	std::cout << "\t\e[31m" << msg <<" \e[0m" << std::endl;
}

inline void				ClientSocket::___debug_request___(int code) const // TODO: DEBUG
{
	std::cout	<< "\e[32m"	<< " \t------------------- \n" \
							<< " \t-     Request     - \n" \
							<< " \t------------------- \e[0m" << std::endl;
	if (code)
		std::cout	<< "\e[35m  ===> CODE: \e[31m /!\\ " << code << " /!\\ \e[0m\n";
	else
		std::cout	<< "\e[35m  ===> CODE: \e[32m " << code << "\e[0m\n";
	std::cout	<< "\e[35m  ===> Request:\e[0m\n";
	std::cout	<< _request << std::endl;
	std::cout	<< "\e[35m  ===> Server:\e[0m\n";
	if (_server)
		std::cout	<< *_server << std::endl;
	else 
		std::cout	<< "\e[31m NULL \e[0m" << std::endl;
	std::cout	<< "\e[35m  ===> Location:\e[0m\n";
	if (_location)
		std::cout	<< *_location << std::endl;
	else
		std::cout	<< "\e[31m NULL \e[0m" << std::endl;
	std::cout	<< "\e[32m"	<< " \t------------------- \e[0m" << std::endl;
}

/*
** ClientSocket
** ================================ Constructors ===============================
*/

ClientSocket::ClientSocket(int port, in_addr_t addr, t_poll& pollfd, Webserv& webserv)
	: ListenSocket(port, addr, pollfd), _webserv(webserv), _request(), _response(),
	_server(NULL), _location(NULL), _reqState(WAITING)
{
	_timer.start();
}

ClientSocket::ClientSocket(ClientSocket const& rhs) // TODO: check usefull ?
	: ListenSocket(rhs._pollfd), _webserv(rhs._webserv), _request(), _response(),
	_server(NULL), _location(NULL), _reqState(WAITING)
{
	*this = rhs;
}

ClientSocket::~ClientSocket() {}

ClientSocket&	ClientSocket::operator=(ClientSocket const& rhs)  // TODO: check usefull ?
{
	if (this != &rhs)
	{
		_port = rhs._port;
		_addr = rhs._addr;
		_pollfd = rhs._pollfd;
		_sockState = rhs._sockState;
		_request = rhs._request;
		_response = rhs._response;
		_server = rhs._server;
		_location = rhs._location;
		_timer = rhs._timer;
		_reqState = rhs._reqState;
	}
	return *this;
}

/*
** ClientSocket
** ============================== Member functions =============================
*/

/*
** int   handleSocket(void);
**
** Handle client's connection from request dowloading to response sending
**
** return: 0 : connection alive
**         1 : connection closed
*/
int				ClientSocket::handleSocket(void)
{
	int		ret;
	std::cout << *this << std::endl; // TODO: DEBUG
	// Client disconnected or Any error
	if (_pollfd.revents & (POLLHUP | POLLERR))
	{
		sockClose();
		___debug_msg___((_sockState == HALF_CLOSED) ? "Closed" : "POLLHUP | POLLERR !");
		return 1;
	}
	// FD not open
	else if (_pollfd.revents & POLLNVAL)
	{
		___debug_msg___("POLLNVAL !");
		if (_timer.getElapsedTime() < TIMEOUT_NVAL)
			return 0;
		sockClose();
		return 1;
	}
	// New Request
	if (_pollfd.revents & POLLIN && _reqState <= RECEIVING)
	{
		// std::cout << "OK OK OK " << std::endl;  // TODO: remove
		ret = _getRequest();
	}
	// Handle Request
	if (_pollfd.revents & POLLOUT && _reqState == SENDING)
	{
		_sendResponse(ret);
	}
	// Check for timeout
	if (_timer.getElapsedTime() > TIMEOUT)
	{
		___debug_msg___("TIMEOUT !");
		sockShutdown();
	}
	return (_sockState == CLOSED);
}

/*
** int   _getRequest(void);
**
** Downloads the request from the client's socket and transfers it to parseRequest()
**
** return:  0 : allright
**         >0 : HTTP error code
*/
int				ClientSocket::_getRequest(void)
{
	int				ret;
	ssize_t			n;
	char			_buff[RECV_BUFF_SIZE + 1];

	n = recv(_pollfd.fd, _buff, RECV_BUFF_SIZE, 0);
	_buff[n] = '\0';
	std::cout << "\e[31m---\n\e[0m" << _buff << "\e[31m---\e[0m" << std::endl; //TODO:remove
	_timer.start();
	std::string		buff = std::string(_buff);
	// Error => Should already be handled by 'handleSocket()'
	if (n < 0)
	{
		sockClose();
		return 0;
	}
	// Gracefull Close
	if (n == 0)
	{
		___debug_msg___("***Gracefull Close***");
		if (_sockState == OPEN)
			sockShutdown();
		else
			sockClose();
		return (_sockState == CLOSED);
	}
	// Can't write anything => no need to process inputs
	if (_sockState == HALF_CLOSED)
		return 0;
	ret = _request.parseRequest(buff);
	// std::cout << "\e[31mret: " << ret << "\e[0m" << std::endl; //TODO:remove
	_reqState = ((_request.isDone() || ret) ? SENDING : RECEIVING);
	return ret;
}

/*
** void  _sendResponse(int code);
**
** code: 0 or an HTTP error code
**
** 1. Find the 'Server' and 'Location';
** 2. Call GetHeaderResponse() to generate a request's response;
** 3. Send the response;
** 4. Decides to close the connection or to keep it alive based on the request
**    or error code.
*/
void			ClientSocket::_sendResponse(int code)
{
	/*
	** TODO: Add the possibility to call Response with an error code number!!
	*/
	int			ret;
	ssize_t		n;
	std::string const* buff;

	ret = _findServer();
	code = (code ? code : ret);
	if (_server)
		code = _findLocation();
	code = (code ? code : ret);
	___debug_request___(code);
	if (code)
		buff = &_response.GetBadRequestResponse(_request, const_cast<Location&>(*_location), code); //TODO: THIS IS UGLY!!!!
	else
		buff = &_response.GetHeaderResponse(_request, const_cast<Location&>(*_location)); //TODO: THIS IS UGLY!!!!
	if (buff->empty()) // TODO: DEBUG ==> How to handle 'Response' error ?
	{
		___debug_msg___("RESPONSE EMPTY !");
		sockClose();
	}
	n = send(_pollfd.fd, buff->c_str(), buff->size(), 0);
	if (n < 0)
	{
		___debug_msg___("SEND ERROR !");
		sockClose();
	}
	
	// std::string	_cnt; // TODO
	// _request.getField("Connection");
	
	_resetSocket();
}

/*
** int   _findServer(void);
**
** Find the request's 'Server' block and update '_server' ptr
*/
int				ClientSocket::_findServer(void)
{
	struct in_addr				addr;
	char						ip[INET_ADDRSTRLEN];
	std::vector<Server const*>	*matchingServers;

	std::string const&			host = _request.getField("Host");
	addr.s_addr = _addr;
	inet_ntop(AF_INET, &addr, ip, INET_ADDRSTRLEN);
	matchingServers = FindMatchingServers(_webserv.getServers(), _port, ip);
	if (matchingServers->empty() || host.empty())
	{
		delete matchingServers;
		return (matchingServers->empty() ? 500 : 400);
	}
	_server = matchingServers->at(0);
	std::cout << std::endl << std::endl << _server->GetServerNames()[0] << std::endl << std::endl; // TODO: remove
	for (size_t i = 0; i < matchingServers->size(); ++i)
	{
		for (size_t j = 0; j < (matchingServers->at(i))->GetServerNames().size(); ++j)
		{
			if (host == (matchingServers->at(i))->GetServerNames()[j])
			{
				_server = matchingServers->at(i);
				delete matchingServers;
				return 0;
			}
		}
	}
	delete matchingServers;
	return 0;
}

/*
** int   _findLocation(void);
**
** Find the request's 'Location' block and update '_location' ptr
*/
int				ClientSocket::_findLocation(void)
{
	/*
	** TODO: GetPath update get a reference!
	*/
	int					len = 0;
	int					max = 0;
	Location const*		candidate = NULL;
	std::string const&	path = _request.getTarget();
	vecLocation const&	locations = _server->GetLocations();

	for (citLocation it = locations.begin(); it < locations.end(); ++it)
	{
		std::string const& loc = it->GetPath();
		if (path.find(loc) == 0)
		{
			len = loc.size();
			if (len > max)
			{
				candidate = &*it;
				max = len;
			}
		}
	}
	_location = candidate;
	if (!_location)
		return 404;
	else if (path[path.size() - 1] != '/')
		return 301;
	else
		return 0;
}

void			ClientSocket::_resetSocket(void)
{
	_request.reset();
	_response.reset();
	_timer.start();
	_reqState = WAITING;
	_server = NULL;
	_location = NULL;
}


/*
** ClientSocket
** ============================ Non-Member functions ===========================
*/

std::ostream&	operator<<(std::ostream& stream, ClientSocket const& sock)
{
	stream << " \e[32m===> ClientSocket \e[0m" << std::endl;
	stream << "     -   Port: " << sock._port << std::endl;
	stream << "     - PollFD: " << sock._pollfd.fd << " ; " \
			<< sock._pollfd.events << " ; " << sock._pollfd.revents;
	return stream;
}
