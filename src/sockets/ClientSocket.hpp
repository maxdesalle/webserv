/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 15:22:19 by tderwedu          #+#    #+#             */
/*   Updated: 2022/03/02 22:09:04 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include "NetworkIPC.hpp"
# include "ListenSocket.hpp"

# define RECV_BUFF_SIZE 1024

class Webserv;

class ClientSocket : public ListenSocket
{
private:
	enum reqState {WAITING, RECEIVING, SENDING};

	Webserv const&		_webserv;
	Request				_request;
	Response			_response;
	Server const		*_server;
	Location const		*_location;
	reqState			_reqState;
	Timer				_timer;

public:
	ClientSocket(ClientSocket const& rhs);
	ClientSocket(int port, in_addr_t addr, t_poll& pollfd, Webserv& webserv);
	virtual ~ClientSocket();
	ClientSocket&				operator=(ClientSocket const& rhs);
private:
	ClientSocket(void);
	
public:
	int							handleSocket(void);

private:
	int							_getRequest(void);
	void						_sendResponse(int code);
	int							_findServer(void);
	int							_findLocation(void);
	void						_resetSocket(void);

	inline void					__debug_request__(int code) const;

public:
	friend std::ostream&		operator<<(std::ostream& stream, ClientSocket const& sock);
};

# include "Webserv.hpp"

#endif
