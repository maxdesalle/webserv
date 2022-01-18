/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxdesalle <mdesalle@student.s19.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 17:28:20 by maxdesall         #+#    #+#             */
/*   Updated: 2022/01/18 18:05:54 by maxdesall        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/webserv.hpp"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Please add one config file as argument when launching the server" << std::endl;
		return (1);
	}
	config_handler(argv[1]);
	return (0);
}
