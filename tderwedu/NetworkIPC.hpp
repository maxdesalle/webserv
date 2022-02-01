/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NetworkIPC.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 16:05:34 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/01 10:10:37 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NETWORKIPC_HPP
# define NETWORKIPC_HPP

// socket(), shutdown(), bind(), listen(), accept(), recv(), send()
# include <arpa/inet.h>
// htons(), htonl(), ntohs(), ntohl(), sockaddr, ...
# include <sys/socket.h>
# include <poll.h>

typedef struct sockaddr_in	t_sockaddr_in;	// Used by `bind` function
typedef struct sockaddr		t_sockaddr;		// Used by `bind` function
typedef struct pollfd		t_poll;			// Used by `poll` function

#endif
