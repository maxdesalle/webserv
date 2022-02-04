/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxdesalle <mdesalle@student.s19.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 18:04:43 by maxdesall         #+#    #+#             */
/*   Updated: 2022/02/04 14:46:00 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

/*
 * Check if the whole line is a comment (which start with a hashtag)
 */

static bool 					FirstCharIsAHashtag(std::string line)
{
	size_t						i = 0;

	while (i < line.size() && std::isblank(line[i]))
		i += 1;
	return (line[i] == '#');
}

static Server					CreateServerObject(std::vector<std::string> ServerContent)
{
	Server						ServerConfig(ServerContent);

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

std::vector<Server>				ConfigHandler(std::string ConfigFileName)
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

static std::vector<size_t>		ReturnDefaultPorts(Server ServerBlock, std::string IP)
{
	std::vector<size_t>			PossiblePorts;

	try
	{
		PossiblePorts = ServerBlock.GetDefaultServer().at(IP);
	}
	catch (std::exception &e)
	{
	}
	return (PossiblePorts);
}

static std::vector<size_t>		ReturnNormalPorts(Server ServerBlock, std::string IP)
{
	std::vector<size_t>			PossiblePorts;

	try
	{
		PossiblePorts = ServerBlock.GetListenIPandPorts().at(IP);
	}
	catch (std::exception &e)
	{
	}
	return (PossiblePorts);
}

static bool						DefaultServerCheck(Server ServerBlock, std::vector<Server> &MatchingServers, size_t Port, std::string IP)
{
	std::vector<size_t>			PossiblePorts;

	PossiblePorts = ReturnDefaultPorts(ServerBlock, IP);
	for (size_t i = 0; i < PossiblePorts.size(); i += 1)
	{
		if (Port == PossiblePorts[i])
		{
			MatchingServers.push_back(ServerBlock);
			return (true);
		}
	}
	return (false);
}

static void						NormalServerCheck(Server ServerBlock, std::vector<Server> &MatchingServers, size_t Port, std::string IP)
{
	std::vector<size_t>			PossiblePorts;

	PossiblePorts = ReturnNormalPorts(ServerBlock, IP);
	for (size_t i = 0; i < PossiblePorts.size(); i += 1)
	{
		if (Port == PossiblePorts[i])
		{
			MatchingServers.push_back(ServerBlock);
			return ;
		}
	}
}

/*
 * Returns a vector of Server objects which match a given port and IP
 */

std::vector<Server>				FindMatchingServers(std::vector<Server> Servers, size_t Port, std::string IP)
{
	std::vector<size_t>			PossiblePorts;
	std::vector<Server>			MatchingServers;

	for (size_t i = 0; i < Servers.size(); i += 1)
		DefaultServerCheck(Servers[i], MatchingServers, Port, IP);
	for (size_t i = 0; i < Servers.size(); i += 1)
		NormalServerCheck(Servers[i], MatchingServers, Port, IP);
	return (MatchingServers);
}
