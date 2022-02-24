/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 17:28:20 by maxdesall         #+#    #+#             */
/*   Updated: 2022/02/24 12:05:55 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>

#include "./config/Config.hpp"
#include "sockets/ClientSocket.hpp"
#include "sockets/NetworkSocket.hpp"
#include "sockets/RequestHandler.hpp"
#include "sockets/Webserv.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Please add one config file as argument when launching the server" << std::endl;
		return (1);
	}
	// (void)argv;
	// std::vector<Server>	Servers;

	// Servers = ConfigHandler(argv[1]);
	// printServers(Servers);
	// _setNetworkSockets(Servers);

	Webserv		server;

	server.initServer(argv[1]);
	server.runWebserv();
	return (0);
}
