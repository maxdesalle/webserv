/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 17:28:20 by maxdesall         #+#    #+#             */
/*   Updated: 2022/03/03 16:20:21 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>

#include "./config/Config.hpp"
#include "sockets/ClientSocket.hpp"
#include "sockets/ListenSocket.hpp"
#include "sockets/Webserv.hpp"

int	main(int argc, char **argv)
{
	Webserv		server;

	if (argc != 2)
		server.initWebserv("conf/default.conf");
	else
		server.initWebserv(argv[1]);
	server.runWebserv();
	return (0);
}
