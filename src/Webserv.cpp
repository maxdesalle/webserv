/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 17:28:20 by maxdesall         #+#    #+#             */
/*   Updated: 2022/02/23 10:51:54 by tderwedu         ###   ########.fr       */
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


	return (0);
}
