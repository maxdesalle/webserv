
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>	//fcntl
#include <fcntl.h>	//fcntl

#include <sys/socket.h>	// socket(), shutdown(), bind(), listen(), accept(), recv(), send()
#include <arpa/inet.h>	// htons(), htonl(), ntohs(), ntohl()
#include <netinet/in.h>
#include <sys/time.h> // struct timeval
#include <poll.h>

#include	<limits.h>	  /* for OPEN_MAX */

#include <iostream>
#include <vector>
#include <map>

typedef std::string			String;
typedef struct pollfd		t_poll;	// Used by `poll` function

class Webserv {
	std::vector<Server>		_servers;
	std::vector<t_poll>		_fd_lst;
	std::vector<Request>	_requests;
	std::vector<Response>	_responses;
		// ...
};

/*
** Dans le fichier config:
**     listen		host:port;
** Si host = localhost => host = 127.0.0.1
*/
class Server {
		String					_server_name;
		int						_port;
		int						_host;
		std::vector<Location>	_locations;
};

class Location {
		String		_root;
		String		_error_page;
		String		_index;
		int			_max_body_size;
		// ...
};

class Request {
	Server	const&	_server;
	t_poll			_poll_fd;
	// ..
};

class Response {
	Server	const&	_server;
	t_poll			_poll_fd;
	// ..
public:
	Response(Request const& req);
};
