/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/01 15:22:19 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/03 09:55:50 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include "NetworkIPC.hpp"
# include "NetworkSocket.hpp"
# include "Request.hpp"

# include <queue>

class ClientSocket : public NetworkSocket
{
private:
	Request			_request;
public:
	ClientSocket(int fd, int port, in_addr_t addr, t_poll const& pollfd);
	virtual ~ClientSocket();
};

ClientSocket::ClientSocket(int fd, int port, in_addr_t addr, t_poll const& pollfd)
: NetworkSocket(fd, port, addr, pollfd)
{}

ClientSocket::~ClientSocket()
{}


#endif
