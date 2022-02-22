/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 17:28:20 by maxdesall         #+#    #+#             */
/*   Updated: 2022/02/22 17:37:32 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Webserv.hpp"

int	main(int argc, char **argv)
{
	std::vector<Server>	Servers;
	std::vector<Server>	*MatchingServers;

	if (argc != 2)
	{
		std::cerr << "Please add one config file as argument when launching the server" << std::endl;
		return (1);
	}

	Servers = ConfigHandler(argv[1]);
	
	printServers(Servers);
	MatchingServers = FindMatchingServers(Servers, 500, "169.0.0.1");
	std::cout << "-----------------------------------" << std::endl;
	if (MatchingServers)
		printServers(*MatchingServers);
	else
		std::cout << "NO MATCHING SERVER!" << std::endl;
	// std::cout << (*MatchingServers)[0].GetServerNames()[0] << std::endl;
	// std::cout << (*MatchingServers)[0].GetLocations()[1].isCgi() << std::endl;
	// std::cout << (*MatchingServers)[0].GetLocations()[0].isMethodValid("POST") << std::endl;
	// MatchingServers = FindMatchingServers(Servers, 5000, "169.38.0.1");
	// std::cout << (*MatchingServers)[0].GetServerNames()[0] << std::endl;
	// MatchingServers = FindMatchingServers(Servers, 8080, "169.38.0.1");
	// std::cout << (*MatchingServers)[0].GetServerNames()[0] << std::endl;
	return (0);
}
