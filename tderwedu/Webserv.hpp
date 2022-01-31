/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 10:19:04 by tderwedu          #+#    #+#             */
/*   Updated: 2022/01/31 11:49:41 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSER_HPP
# define WEBSERV_HPP

# include <unistd.h>		// fcntl, sysconf
# include <errno.h>
# include <fcntl.h>			// fcntl
# include <sys/socket.h>	// socket(), shutdown(), bind(), listen(), accept(), recv(), send()
# include <arpa/inet.h>		// htons(), htonl(), ntohs(), ntohl()
# include <netinet/in.h>
# include <sys/time.h> 		// struct timeval
# include <limits.h>		// for OPEN_MAX
# include <poll.h>

# include <iostream>
# include <vector>
# include <map>
# include "Server.hpp"
# include "Location.hpp"
# include "Request.hpp"
# include "Response.hpp"


typedef std::string			String;
typedef struct pollfd		t_poll;	// Used by `poll` function

# ifdef OPEN_MAX
	static long	open_max = OPEN_MAX;
# else
	static long	open_max = 0;
# endif
# define OPEN_MAX_GUESS	256

class Webserv
{
private:
	std::vector<Server>		_servers;
	std::vector<t_poll>		_fd_lst;
	std::vector<Request>	_requests;
	std::vector<Response>	_responses;
public:
	Webserv();
	~Webserv();

	void	setOpenMax(void);
};

Webserv::Webserv(/* args */)
{
}

Webserv::~Webserv()
{
}

void	Webserv::setOpenMax(void)
{
	if (!open_max)
	{
		errno = 0;
		if ((open_max = sysconf(_SC_OPEN_MAX)) < 0)
		{
			if (errno == 0)
				open_max = OPEN_MAX_GUESS;
			else
				std::cerr << "sysconf error for _SC_OPEN_MAX" << std::endl;	//TODO: better error handling
		}
	}
	
}

#endif
