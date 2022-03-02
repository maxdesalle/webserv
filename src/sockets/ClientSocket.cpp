/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:55:52 by tderwedu          #+#    #+#             */
/*   Updated: 2022/03/02 19:00:18 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientSocket.hpp"

inline static void		__debug_msg__(char const *msg) // TODO: DEBUG
{
	std::cout << "\t\e[31m" << msg <<" \e[0m" << std::endl;
}

inline void				ClientSocket::__debug_request__(void) const // TODO: DEBUG
{
	std::cout	<< "\e[32m"	<< " \t------------------- \n" \
							<< " \t-     Request     - \n" \
							<< " \t------------------- \e[0m" << std::endl;
	std::cout	<< _request << std::endl;
	if (_server)
	{
		std::cout	<< "\e[34m  ===> Server:\e[0m\n";
		std::cout	<< _server << std::endl;
	}
	if (_location)
	{
		std::cout	<< "\e[34m  ===> Location:\e[0m\n";
		std::cout	<< _location << std::endl;
	}
	std::cout	<< "\e[32m"	<< " \t------------------- " << std::endl;
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
		__debug_msg__("POLLHUP | POLLERR !");
		sockClose();
		return 1;
	}
	// FD not open
	else if (_pollfd.revents & POLLNVAL)
	{
		__debug_msg__("POLLNVAL !");
		if (_timer.getElapsedTime() < TIMEOUT_NVAL)
			return 0;
		sockClose();
		return 1;
	}
	// New Request
	// std::cout << "_pollfd.revents & POLLIN: " << (_pollfd.revents & POLLIN) << std::endl; // TODO: remove
	// std::cout << "_reqState <= DOWNLOADING: " << (_reqState <= DOWNLOADING) << std::endl; // TODO: remove
	if (_pollfd.revents & POLLIN && _reqState <= DOWNLOADING)
	{
		// std::cout << "OK OK OK " << std::endl;  // TODO: remove
		ret = _getRequest();
	}
	// Handle Request
	if (_pollfd.revents & POLLOUT && _reqState >= PROCESSING)
	{
		_sendResponse(ret);
	}
	// Check for timeout
	if (_timer.getElapsedTime() > TIMEOUT)
	{
		__debug_msg__("TIMEOUT !");
		sockShutdown();
	}
	return (_sockState == CLOSED);
}

int				ClientSocket::_getRequest(void)
{
	int				ret;
	ssize_t			n;
	char			_buff[RECV_BUFF_SIZE + 1];

	n = recv(_pollfd.fd, _buff, RECV_BUFF_SIZE, 0);
	_buff[n] = '\0';
	_timer.start();
	std::string		buff = std::string(_buff);
	std::cout << "OK OK OK : " << buff << std::endl;  // TODO: remove
	// Error => Should already be handled by `handleSocket()`
	if (n < 0)
	{
		sockClose();
		return 0;
	}
	// Gracefull Close
	if (n == 0)
	{
		__debug_msg__("*Gracefull Close*");
		if (_sockState == OPEN)
			sockClose();
		else
			sockShutdown();
		return 0;
	}
	// Can't write anything => no need to process inputs
	if (_sockState == HALF_CLOSED)
		return 0;
	ret = _request.parseRequest(buff);
	_reqState = (ret ? PROCESSING : DOWNLOADING);
	return ret;
}

void			ClientSocket::_sendResponse(int code)
{
	/*
	** TODO: Add the possibility to call Response with an error code number!!
	*/
	int 		ret;
	ssize_t		n;

	(void)code;
	ret = _findServer();
	if (!ret)
		ret = _findLocation();

	__debug_request__();

	std::string const& buff = _response.GetHeaderResponse(_request, const_cast<Location&>(*_location)); //TODO: THIS IS UGLY!!!!
	n = send(_pollfd.fd, buff.c_str(), buff.size(), 0);
	if (n < 0)
	{
		__debug_msg__("SEND ERROR !");
		sockClose();
	}
	_resetSocket();
}

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

int				ClientSocket::_findLocation(void)
{
	/*
	** TODO: GetPath update get a reference!
	*/
	int					len = 0;
	int					max = 0;
	Location const		*candidate = NULL;
	std::string const&	path = _request.getTarget();
	vecLocation const&	locations = _server->GetLocations();
	std::string			loc;

	for (citLocation it = locations.begin(); it < locations.end(); ++it)
	{
		loc = it->GetPath();
		if (path.find(loc) == 0)
		{
			len = (loc).size();
			if (len > max)
			{
				candidate = &*it;
				max = len;
			}
		}
	}
	_location = candidate;
	return (_location ? 404 : 0);
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
