/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 15:54:27 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/01 10:32:21 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORKSOCKET_HPP
# define NETWORKSOCKET_HPP

# include <unistd.h>

# include "NetworkIPC.hpp"
# include "Timer.hpp"

struct NetworkSocket
{
public:
	enum Status {OPEN, HALF_CLOSED, CLOSED};

	int				fd;
	int				port;
	in_addr_t		addr;
	t_poll const&	pollfd;
	Status			status;
	Timer			timer;

	NetworkSocket(int fd, int port, in_addr_t addr, t_poll const& pollfd);
	virtual ~NetworkSocket(void);

	void	gracefullClose(void);
	void	errorClose(void);
};

NetworkSocket::NetworkSocket(int fd, int port, in_addr_t addr, t_poll const& pollfd)
: fd(fd), port(port), addr(addr), pollfd(pollfd), status(OPEN)
{
	timer.start();
}

NetworkSocket::~NetworkSocket() {}

void	NetworkSocket::gracefullClose(void)
{
	if (status == OPEN)
	{
		shutdown(fd, SHUT_WR);
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
