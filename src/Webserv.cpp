/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 17:28:20 by maxdesall         #+#    #+#             */
/*   Updated: 2022/03/16 14:59:07 by tderwedu         ###   ########.fr       */
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
	std::cout	<< "\e[32m" \
				<< " \t##########################################################\n" \
				<< " \t#                      W E B S E R V                     #\n" \
				<< " \t##########################################################\e[0m" \
				<< std::endl;

	if (argc != 2)
		server.initWebserv("default/website.conf");
	else
		server.initWebserv(argv[1]);
	server.runWebserv();
	return (0);
}
