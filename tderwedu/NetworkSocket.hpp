/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 15:54:27 by tderwedu          #+#    #+#             */
/*   Updated: 2022/01/31 17:57:28 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORKSOCKET_HPP
# define NETWORKSOCKET_HPP

# include <unistd.h>

# include "NetworkIPC.hpp"

struct NetworkSocket
{
public:
	enum Status {OPEN, HALF_CLOSED, CLOSED};

	int		port;
	int		fd;
	Status	status;

	NetworkSocket(int port, int fd);
	virtual ~NetworkSocket(void);

	void	gracefullClose(void);
	void	errorClose(void);
};

NetworkSocket::NetworkSocket(int port, int fd)
: port(port), fd(fd), status(OPEN)
{}

NetworkSocket::~NetworkSocket() {}

void	NetworkSocket::gracefullClose(void)
{
	if (status == OPEN)
	{
		shutdown(fd, SHUT_RD);
		status = HALF_CLOSED;
	}
	else
	{
		shutdown(fd, SHUT_RDWR);
		close(fd);
		status = CLOSED;
	}
}

void	NetworkSocket::errorClose(void)
{
	shutdown(fd, SHUT_RDWR);
	close(fd);
	status = CLOSED;
}


#endif
