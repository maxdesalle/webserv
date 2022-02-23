/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 15:22:19 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/23 19:25:18 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include "NetworkIPC.hpp"
# include "NetworkSocket.hpp"
# include "RequestHandler.hpp"

# define	BUFF_SIZE	1024

class Webserv;
class RequestHandler;
typedef std::deque<RequestHandler>		lstReqHand;
typedef lstReqHand::iterator			itReqHand;

class ClientSocket : public NetworkSocket
{
public:
	static const int	buffSize = BUFF_SIZE;
private:
	Webserv&			_webserv;
	lstReqHand			_messages;
	char				_buff[BUFF_SIZE];
	Timer				_timer;

public:
	ClientSocket(int port, in_addr_t addr, t_poll& pollfd, Webserv& webserv);
	virtual ~ClientSocket();
	ClientSocket&	operator=(ClientSocket const& rhs);

	void			getNewRequest(void);
	void			getRessources(void);

	int				empty(void);
private:
	void			_findServer(void);
	void			_clearSocket(void);
};

# include "Webserv.hpp"

#endif
