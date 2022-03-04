/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 16:13:26 by tderwedu          #+#    #+#             */
/*   Updated: 2022/03/04 10:28:49 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORKSOCKET_HPP
# define NETWORKSOCKET_HPP

# include "NetworkIPC.hpp"

# define	TIMEOUT			3 * 60 * 1000 // 3 * 60 * 1000
# define	TIMEOUT_NVAL	100

class ListenSocket
{
public:
	enum sockState {OPEN, HALF_CLOSED, CLOSED};
protected:
	t_poll&			_pollfd;
	int				_port;
	in_addr_t		_addr;
	sockState		_sockState;

public:
	ListenSocket(t_poll& pollfd);
	ListenSocket(ListenSocket const& rhs);
	ListenSocket(int port, in_addr_t addr, t_poll& pollfd);
	virtual ~ListenSocket(void);
	ListenSocket&			operator=(ListenSocket const& rhs);
private:
	ListenSocket(void);

public:
	in_addr_t				getIP(void);
	int						getPort(void);
	t_poll&					getPollFd(void);
	void					sockShutdown(int how);
	void					sockClose(void);

	friend std::ostream&	operator<<(std::ostream& stream, ListenSocket const& sock);
};

#endif
