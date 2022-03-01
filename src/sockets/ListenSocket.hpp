/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 16:13:26 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/24 19:50:34 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORKSOCKET_HPP
# define NETWORKSOCKET_HPP

# include "NetworkIPC.hpp"

# define	TIMEOUT			3 * 60 * 1000
# define	TIMEOUT_NVAL	100

class ListenSocket
{
public:
	enum State {OPEN, HALF_CLOSED, CLOSED};
protected:
	t_poll&			_pollfd;
	int				_port;
	in_addr_t		_addr;
	State			_state;

public:
	ListenSocket(t_poll& pollfd);
	ListenSocket(int port, in_addr_t addr, t_poll& pollfd);
	ListenSocket(ListenSocket const& rhs);
	virtual ~ListenSocket(void);
	ListenSocket&	operator=(ListenSocket const& rhs);

	in_addr_t	getIP(void);
	int			getPort(void);
	t_poll&		getPollFd(void);
	State		getState(void);
	bool		isOpen(void) const;

	void		sockShutdown(void);
	void		sockClose(void);

	friend std::ostream&			operator<<(std::ostream& stream, ListenSocket const& sock);
private:
	ListenSocket(void);
};

#endif
