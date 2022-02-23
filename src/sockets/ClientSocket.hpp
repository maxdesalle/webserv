/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 15:22:19 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/23 11:03:08 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <deque>



# include "NetworkIPC.hpp"

# include "RequestHandler.hpp"


# include "../HTTP_Config/Request.hpp"

# define	BUFF_SIZE	1024

class ClientSocket : public NetworkSocket
{
	typedef std::deque<RequestHandler>	cont_reqHand;
	typedef cont_reqHand::iterator		it_reqHand;
public:
	static const int	buffSize = BUFF_SIZE;
private:
	Webserv&			_webserv;
	cont_reqHand		_messages;
	char				_buff[BUFF_SIZE];
	Timer				_timer;
	State				_state;
	

public:
	ClientSocket(int port, in_addr_t addr, t_poll& pollfd, Webserv& webserv);
	virtual ~ClientSocket();

	void		getNewRequest(void);
	void		getRessources(void);

	int			empty(void);
private:
	void		_findServer(void);
	void		_clearSocket(void);
};

#endif
