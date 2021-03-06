/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 10:19:04 by tderwedu          #+#    #+#             */
/*   Updated: 2022/03/08 01:03:14 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <vector>
# include <deque>
# include <list>
# include <map>
# include <set>

# include "NetworkIPC.hpp"
# include "ListenSocket.hpp"
# include "ClientSocket.hpp"

# ifdef OPEN_MAX
	static size_t	open_max = OPEN_MAX;
# else
	static size_t	open_max = 0;
# endif

# define OPEN_MAX_GUESS		256
# define POLL_FLAGS			POLLIN | POLLOUT

typedef std::vector<ListenSocket>		vecListSock;
typedef vecListSock::iterator			itlistSock;
typedef std::list<ClientSocket>			lstClieSock;
typedef lstClieSock::iterator			itClieSock;

class Webserv
{
private:
	size_t						_fdInUse, _nbrPorts, _nbrPoll, _nbrPollMax;
	vecServer					_servers;
	t_poll						*_pollfd;
	vecListSock					_listenSocks;
	lstClieSock					_clientSocks;

public:
	Webserv(void);
	~Webserv(void);
private:
	Webserv(Webserv const& rhs);
	Webserv&			operator=(Webserv const& rhs);

public:
	vecServer const&	getServers(void) const;
	void				initWebserv(std::string const& config);
	void				runWebserv(void);
private:
	void				_setOpenMax(void) const;
	void				_setNetworkSockets(void);
	void				_addNetworkSocket(int port, int nbr_queue);
	void				_checkListenSockets(void);
	void				_checkClientSockets(void);
	t_poll&				_pushPollfd(int fd_client);
	void				_popPollfd(t_poll& pollfd);

	size_t				__getNbrPollListen(void) const;
	size_t				__getNbrPollClient(void) const;
	inline void			___debug_before_poll___(void) const;
	inline void			___debug_after_poll___(void) const;
};

#endif
