/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 17:28:20 by maxdesall         #+#    #+#             */
/*   Updated: 2022/03/01 09:38:32 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>

#include "./config/Config.hpp"
#include "sockets/ClientSocket.hpp"
#include "sockets/ListenSocket.hpp"
#include "sockets/RequestHandler.hpp"
#include "sockets/Webserv.hpp"

int	main(int argc, char **argv)
{
	Webserv		server;

	if (argc != 2 && argv[0])
	{
		std::cerr << "Please add one config file as argument when launching the server" << std::endl;
		return (1);
	}
	server.initWebserv(argv[1]);
	server.runWebserv();
	return (0);
}
