/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 10:19:04 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/23 11:12:49 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSER_HPP
# define WEBSERV_HPP

# include <iostream>
# include <vector>
# include <map>

# include "NetworkIPC.hpp"

# ifdef OPEN_MAX
	static long	open_max = OPEN_MAX;
# else
	static long	open_max = 0;
# endif
# define OPEN_MAX_GUESS		256
# define POLL_FLAGS			POLLIN | POLLOUT	// POLLERR | POLLHUP | POLLNVAL are allways set

typedef std::vector<Server>					cont_server;
typedef std::vector<NetworkSocket>			cont_netSock;
typedef std::vector<ClientSocket>			cont_cliSock;
typedef cont_netSock::iterator				it_netSock;
typedef cont_cliSock::iterator				it_cliSock; // TODO: Change to deque

class Webserv
{
private:
	static std::string const	_validMethod[4];
	int							_fdInUse;
	std::vector<t_poll>			_pollfd;
	cont_server					_servers;
	cont_netSock				_serverSocks;
	cont_cliSock				_clientSocks;
	
public:
	Webserv(void);
	~Webserv(void);

	void				setOpenMax(void);

	cont_server&		getServers(void);
	
	void				addServerSocket(int port, int nbr_queue);

	void				checkServerSockets(void);
	void				checkClientSockets(void);
	void				reapClosedClientSock(void);
	
	t_poll&				addPollfd(int fd_client);
	void				popPollfd(t_poll& pollfd);

	int					isValidRequest(const std::string& method);

};

#endif
