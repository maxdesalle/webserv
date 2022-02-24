/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxdesalle <mdesalle@student.s19.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 17:28:20 by maxdesall         #+#    #+#             */
/*   Updated: 2022/02/21 14:47:25 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Webserv.hpp"

int	main(int argc, char **argv)
{
	/* std::vector<Server>	Servers; */
	/* std::vector<Server>	*MatchingServers; */

	if (argc != 2 && argv[0])
	{
		std::cerr << "Please add one config file as argument when launching the server" << std::endl;
		return (1);
	}

	/* Servers = ConfigHandler(argv[1]); */
	/* MatchingServers = FindMatchingServers(Servers, 3000, "127.0.0.1"); */
	Response	test;
	std::cout << test.GetCurrentFormattedTime() << std::endl;
	return (0);
}
