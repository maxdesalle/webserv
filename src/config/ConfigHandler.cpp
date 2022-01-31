/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxdesalle <mdesalle@student.s19.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 18:04:43 by maxdesall         #+#    #+#             */
/*   Updated: 2022/01/28 17:21:07 by mdesalle         ###   ########.fr       */
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
	Server	config(file_name);
	/* std::map<std::string, std::string>::iterator it; */
	
	/* for (size_t i = 0; i < config.GetConfigFileContent().size(); i += 1) */
	/* 	std::cout << config.GetConfigFileContent()[i] << std::endl; */
		
	/* std::cout << "listen: " << config.GetServers()[0]["listen"] << std::endl; */
	/* std::cout << "server_name: " << config.GetServers()[0]["server_name"] << std::endl; */
	/* std::cout << "root: " << config.GetServers()[0]["root"] << std::endl; */
	/* std::cout << "error_page 404: " << config.GetServers()[0]["error_page 404"] << std::endl; */
	/* std::cout << "return 301: " << config.GetServers()[0]["return 301"] << std::endl; */
	/* std::cout << "location /get: " << config.GetServers()[0]["location /get"] << std::endl; */
	/* std::cout << "location /post: " << config.GetServers()[0]["location /post"] << std::endl; */
	/* std::cout << "location /put: " << config.GetServers()[0]["location /put"] << std::endl; */

	/* std::vector<std::string>			test; */

	/* test = config.GetLocations(0); */
	/* for (size_t i = 0; i < test.size(); i += 1) */
	/* 	std::cout << test[i] << std::endl; */
	
	/* for (size_t i = 0; i < config.GetServers().size(); i += 1) */
	/* { */
	/* 	for (it = config.GetServers()[i].begin(); it != config.GetServers()[i].end(); it++) */
	/* 		std::cout << it->first << ":" << it->second << std::endl; */
	/* 	std::cout << std::endl; */
	/* } */

	/* int sizeMap = static_cast<int>(config.GetServers()[0].size()); */
	/* it = config.GetServers()[0].begin(); */
	/* for(int i = 0; i < sizeMap; i++) */
	/* { */
	/* 	std::cout << "Key: " << it->first << " Value: " << it->second << std::endl; */
	/* 	it++; */
	/* } */
}
