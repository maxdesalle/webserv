/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxdesalle <mdesalle@student.s19.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 18:04:43 by maxdesall         #+#    #+#             */
/*   Updated: 2022/02/03 12:52:17 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

/*
 * Check if the whole line is a comment (which start with a hashtag)
 */

static bool 				FirstCharIsAHashtag(std::string line)
{
	size_t					i = 0;

	while (i < line.size() && std::isblank(line[i]))
		i += 1;
	return (line[i] == '#');
}

static Server				CreateServerObject(std::vector<std::string> ServerContent)
{
	Server					ServerConfig(ServerContent);

	return (ServerConfig);
}

/*
 * Returns a vector of Server objects which match the given Port and IP address
 */

/* std::vector<Server>			FindMatchingServers(size_t Port, std::string IP) */
/* { */
/* } */

/*
 * Returns a vector with a Server object for each server block in the provided config file
 */

std::vector<Server>	ConfigHandler(std::string ConfigFileName)
{
	std::string					Line;
	std::ifstream				File(ConfigFileName);
	std::vector<std::string>	ServerContent;
	std::vector<Server>			Servers;

	if (!File.is_open())
	{
		std::cout << "File could not be opened";
		exit(1);
	}

	while (std::getline(File, Line))
	{
		if (Line.rfind("server ", 0) == 0 && !ServerContent.empty())
		{
			Servers.push_back(CreateServerObject(ServerContent));
			ServerContent.clear();
		}
		if (!Line.empty() && !FirstCharIsAHashtag(Line))
			ServerContent.push_back(Line);
	}
	Servers.push_back(CreateServerObject(ServerContent));

	File.close();
	return (Servers);
}
