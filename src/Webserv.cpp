/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 17:28:20 by maxdesall         #+#    #+#             */
/*   Updated: 2022/02/23 15:18:11 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>

#include "./config/Config.hpp"
// #include "sockets/NetworkIPC.hpp"


#include <set>
void	_setNetworkSockets(std::vector<Server>	const& servers)
{
	std::set<int>	ports;
	mapServer		ip_ports;

	for (citServer serv = servers.begin(); serv != servers.end(); ++serv)
	{
		ip_ports = serv->GetListenIPandPorts();
		for (mapServer::const_iterator cit = ip_ports.begin(); cit != ip_ports.end(); ++cit)
		{
			for (std::vector<size_t>::const_iterator port = cit->second.begin(); port != cit->second.end(); ++port)
				ports.insert(*port);
		}
	}
	// DEBUG
	std::cout << "Ports: ";
	for (std::set<int>::const_iterator cit = ports.begin(); cit != ports.end(); ++cit)
	{
		std::cout << *cit << " ";
	}
	std::cout << std::endl;
}

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Please add one config file as argument when launching the server" << std::endl;
		return (1);
	}
	(void)argv;
	std::vector<Server>	Servers;
	Servers = ConfigHandler(argv[1]);
	printServers(Servers);
	_setNetworkSockets(Servers);

	// server.initServer(argv[1]);
	// Webserv		server;
	return (0);
}
