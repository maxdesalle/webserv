/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxdesalle <mdesalle@student.s19.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 18:04:43 by maxdesall         #+#    #+#             */
/*   Updated: 2022/01/20 15:46:09 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

/*
 * main config function which handles the whole config process which includes
 * reading from the config file, checking the read information and saving it
 * in data structures
 */

void	ConfigHandler(char *file_name)
{
	ConfigReader	config(file_name);

	/* for (size_t i = 0; i < config.GetServers().size(); i += 1) */
	/* { */
	/* 	for (size_t j = 0; j < config.GetServers()[i].size(); j += 1) */
	/* 		std::cout << config.GetServers()[i][j] << std::endl; */
	/* 	std::cout << std::endl; */
	/* } */
}
