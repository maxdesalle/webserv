/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxdesalle <mdesalle@student.s19.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 17:28:20 by maxdesall         #+#    #+#             */
/*   Updated: 2022/02/02 17:52:04 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Webserv.hpp"

int	main(int argc, char **argv)
{
	std::vector<Server>	Servers;

	if (argc != 2)
	{
		std::cerr << "Please add one config file as argument when launching the server" << std::endl;
		return (1);
	}

	Servers = ConfigHandler(argv[1]);
	std::cout << Servers[1].GetLocations()[0].GetAllowMethod()[0] << std::endl;
	std::cout << Servers[0].GetLocations()[1].GetRoot() << std::endl;
	
	return (0);
}
