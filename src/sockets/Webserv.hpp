/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 10:19:04 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/23 19:19:11 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <vector>
# include <map>

# include "NetworkIPC.hpp"
# include "NetworkSocket.hpp"
# include "ClientSocket.hpp"

# ifdef OPEN_MAX
	static size_t	open_max = OPEN_MAX;
# else
	static size_t	open_max = 0;
# endif

# define OPEN_MAX_GUESS		256
# define POLL_FLAGS			POLLIN | POLLOUT	// POLLERR | POLLHUP | POLLNVAL are allways set

typedef std::vector<NetworkSocket>		vecNetSock;
typedef vecNetSock::iterator			itNetSock;
typedef std::deque<ClientSocket>		lstClieSock;
typedef lstClieSock::iterator			itClieSock;

class Webserv
{
private:
	static std::string const	_validMethod[4];
	size_t						_fdInUse;
	vecServer					_servers;
	std::vector<t_poll>			_pollfd;
	vecNetSock					_serverSocks;
	lstClieSock					_clientSocks;
	
	Webserv&	operator=(Webserv const& rhs);
public:
	Webserv(void);
	~Webserv(void);

	void				_setOpenMax(void) const;
	void				initServer(std::string const& config);

	vecServer&			getServers(void);
	
	void				addServerSocket(int port, int nbr_queue);

	void				checkServerSockets(void);
	void				checkClientSockets(void);
	void				reapClosedClientSock(void);
	
	t_poll&				addPollfd(int fd_client);
	void				popPollfd(t_poll& pollfd);

	int					isValidRequest(const std::string& method);

};

#endif
