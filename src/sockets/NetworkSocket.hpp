/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkSocket.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 15:54:27 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/23 17:36:35 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORKSOCKET_HPP
# define NETWORKSOCKET_HPP

# include "NetworkIPC.hpp"

# define	TIMEOUT			3 * 60 * 1000
# define	TIMEOUT_NVAL	100

class NetworkSocket
{
public:
	enum State {OPEN, HALF_CLOSED, CLOSED};
protected:
	int				_port;
	in_addr_t		_addr;
	t_poll&			_pollfd;
	State			_state;

public:
	NetworkSocket(int port, in_addr_t addr, t_poll& pollfd);
	virtual ~NetworkSocket(void);
	NetworkSocket&	operator=(NetworkSocket const& rhs);

	in_addr_t	getIP(void);
	int			getPort(void);
	t_poll&		getPollFd(void);
	State		getState(void);
	bool		isOpen(void) const;

	void		sockShutdown(void);
	void		sockClose(void);

	void		handlePollErr(void);
};

#endif
