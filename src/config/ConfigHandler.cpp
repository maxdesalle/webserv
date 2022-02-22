/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 18:04:43 by maxdesall         #+#    #+#             */
/*   Updated: 2022/02/22 17:27:42 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

/*
 * Check if the whole line is a comment (which start with a hashtag)
 */

static bool 					FirstCharIsAHashtag(std::string line)
{
	size_t						i = 0;

	while (i < line.size() && std::isspace(line[i]))
		i += 1;
	return (line[i] == '#');
}

static Server					CreateServerObject(std::vector<std::string> ServerContent)
{
	Server						ServerConfig(ServerContent);

	return (ServerConfig);
}

/*
 * Returns a vector with a Server object for each server block in the provided config file
 */

std::vector<Server>				ConfigHandler(std::string ConfigFileName)
{
	std::string					Line;
	std::ifstream				File(ConfigFileName.c_str());
	std::vector<std::string>	ServerContent;
	std::vector<Server>			Servers;

	if (!File.is_open())
	{
		std::cout << "File could not be opened";
		exit(1);
	}

	while (std::getline(File, Line))
	{
		std::replace(Line.begin(), Line.end(), '\t', ' ');
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
	std::vector<size_t>									PossiblePorts;
	std::map<std::string, std::vector<size_t> > const&	defaultServers = ServerBlock.GetDefaultServer();

	for (std::map<std::string, std::vector<size_t> >::const_iterator it = defaultServers.begin(); it != defaultServers.end(); ++it)
	{
		if (it->first == "0.0.0.0")
			PossiblePorts.insert(PossiblePorts.end(), it->second.begin(), it->second.end());
		else if (it->first == IP)
			PossiblePorts.insert(PossiblePorts.end(), it->second.begin(), it->second.end());
	}
	return (PossiblePorts);
}

static std::vector<size_t>		ReturnNormalPorts(Server ServerBlock, std::string IP)
{
	std::vector<size_t>									PossiblePorts;
	std::map<std::string, std::vector<size_t> > const&	listen = ServerBlock.GetListenIPandPorts();

	for (std::map<std::string, std::vector<size_t> >::const_iterator it = listen.begin(); it != listen.end(); ++it)
	{
		if (it->first == "0.0.0.0")
			PossiblePorts.insert(PossiblePorts.end(), it->second.begin(), it->second.end());
		else if (it->first == IP)
			PossiblePorts.insert(PossiblePorts.end(), it->second.begin(), it->second.end());
	}
	return (PossiblePorts);
}

// static std::vector<size_t>		ReturnDefaultPorts(Server ServerBlock, std::string IP)
// {
// 	std::vector<size_t>			PossiblePorts;

// 	try
// 	{
// 		PossiblePorts = ServerBlock.GetDefaultServer().at(IP);
// 	}
// 	catch (std::exception &e)
// 	{
// 	}
// 	return (PossiblePorts);
// }

// static std::vector<size_t>		ReturnNormalPorts(Server ServerBlock, std::string IP)
// {
// 	std::vector<size_t>			PossiblePorts;

// 	try
// 	{
// 		PossiblePorts = ServerBlock.GetListenIPandPorts().at(IP);
// 	}
// 	catch (std::exception &e)
// 	{
// 	}
// 	return (PossiblePorts);
// }

static bool						DefaultServerCheck(Server ServerBlock, std::vector<Server> *MatchingServers, size_t Port, std::string IP)
{
	std::vector<size_t>			PossiblePorts;

	PossiblePorts = ReturnDefaultPorts(ServerBlock, IP);
	for (size_t i = 0; i < PossiblePorts.size(); i += 1)
	{
		if (Port == PossiblePorts[i])
		{
			MatchingServers->push_back(ServerBlock);
			return (true);
		}
	}
	return (false);
}

static void						NormalServerCheck(Server ServerBlock, std::vector<Server> *MatchingServers, size_t Port, std::string IP)
{
	std::vector<size_t>			PossiblePorts;

	PossiblePorts = ReturnNormalPorts(ServerBlock, IP);
	for (size_t i = 0; i < PossiblePorts.size(); i += 1)
	{
		if (Port == PossiblePorts[i])
		{
			MatchingServers->push_back(ServerBlock);
			return ;
		}
	}
}

/*
 * Returns a vector of Server objects which match a given port and IP
 */

std::vector<Server>				*FindMatchingServers(std::vector<Server> &Servers, size_t Port, std::string IP)
{
	std::vector<size_t>			PossiblePorts;
	std::vector<Server>			*MatchingServers = new std::vector<Server>;

	for (size_t i = 0; i < Servers.size(); i += 1)
		DefaultServerCheck(Servers[i], MatchingServers, Port, IP);
	for (size_t i = 0; i < Servers.size(); i += 1)
		NormalServerCheck(Servers[i], MatchingServers, Port, IP);
	return (MatchingServers);
}

void						printServers(std::vector<Server> &Servers)
{
	std::cout << "Number of servers: " << Servers.size() << std::endl;
	for (size_t i = 0; i < Servers.size(); ++i)
		Servers[i].printServer();
}
