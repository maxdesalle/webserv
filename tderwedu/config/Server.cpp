/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesalle <mdesalle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 14:07:11 by mdesalle          #+#    #+#             */
/*   Updated: 2022/02/02 18:50:10 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//======================== CONSTRUCTORS / DESTRUCTORS ========================//

Server::Server(std::vector<std::string> ConfigFileContent)
{
	CommentEraser(ConfigFileContent);
	FrontSpaceEraser(ConfigFileContent);
	ErrorChecker(ConfigFileContent);
	SemiColonRemover(ConfigFileContent);
	LocationScopeAssembler(ConfigFileContent);
	FindListenPorts(ConfigFileContent);
	FindServerNames(ConfigFileContent);
	AddOutsideInfoToLocation(ConfigFileContent);
	LocationFinder(ConfigFileContent);
}

Server::Server(Server const &ref)
{
	*this = ref;
}

Server::~Server(void)
{
}

//================================ OPERATORS =================================//

Server						&Server::operator=(Server const &ref)
{
	this->_ListenPorts = ref.GetListenPorts();
	this->_ServerNames = ref.GetServerNames();
	this->_Locations = ref.GetLocations();
	return (*this);
}

//============================ GETTERS / SETTERS =============================//

std::vector<size_t>			Server::GetListenPorts(void)	const
{
	return (_ListenPorts);
}

std::vector<std::string>	Server::GetServerNames(void)	const
{
	return (_ServerNames);
}

std::vector<Location>		Server::GetLocations(void)		const
{
	return (_Locations);
}

//================================ FUNCTIONS =================================//

void						Server::WriteErrorMessage(std::string message)	const
{
	std::cerr << message << std::endl;
	exit(1);
}

/*
 * Remove all comments from config file
 */

void						Server::CommentEraser(std::vector<std::string> &ConfigFileContent)
{
	for (size_t i = 0; i < ConfigFileContent.size(); i += 1)
	{
		for (size_t j = 0; j < ConfigFileContent[i].size(); j += 1)
		{
			if (ConfigFileContent[i][j] == '#')
			{
				ConfigFileContent[i].erase(j, ConfigFileContent[i].size());
				break ;
			}
		}
	}
}

/*
 * Removes all the unnecessary space int front of the config settings
 */

void						Server::FrontSpaceEraser(std::vector<std::string> &ConfigFileContent)
{
	size_t								j = 0;

	for (size_t i = 0; i < ConfigFileContent.size(); i += 1)
	{
		while (j < ConfigFileContent[i].size() && std::isblank(ConfigFileContent[i][j]))
			j += 1;
		ConfigFileContent[i].erase(0, j);
		j = 0;
	}
}

/*
 * Check if there are no configuration settings outside of a "server" context
 */

bool						Server::OutOfServerContextCheck(std::vector<std::string> &ConfigFileContent)	const
{
	size_t											i = 0;

	if (ConfigFileContent[0].rfind("server {", 0))
		return (false);
	while (i < ConfigFileContent.size() - 1)
	{
		i = ReturnEndOfScopeLineNumber(i, ConfigFileContent) + 1;
		if (i != ConfigFileContent.size())
		{
			if (ConfigFileContent[i].rfind("server {", 0))
				return (false);
		}
	}
	return (true);
}

/*
 * Returns the line number where the bracket located in the line number passed as argument
 * is closed (so the scope of the brackets is closed)
 */

size_t						Server::ReturnEndOfScopeLineNumber(size_t BeginBracketsLineNumber, std::vector<std::string> &ConfigFileContent)	const
{
	size_t											NumberOfOpenScopes = 1;

	for (size_t i = BeginBracketsLineNumber + 1; i < ConfigFileContent.size(); i += 1)
	{
		for (size_t j = 0; j < ConfigFileContent[i].size(); j += 1)
		{
			if (ConfigFileContent[i][j] == '{')
				NumberOfOpenScopes += 1;
			else if (ConfigFileContent[i][j] == '}')
				NumberOfOpenScopes -= 1;

			if (NumberOfOpenScopes == 0)
				return (i);
		}
	}
	return (0);
}

/*
 * Check if each bracket left bracket { is matched with a right bracket }
 */

bool						Server::CheckBrackets(std::vector<std::string> &ConfigFileContent)	const
{
	size_t								LeftBracketCounter = 0;
	size_t								RightBracketCounter = 0;

	for (size_t i = 0; i < ConfigFileContent.size(); i += 1)
	{
		for (size_t j = 0; j < ConfigFileContent[i].size(); j += 1)
		{
			if (ConfigFileContent[i][j] == '{')
				LeftBracketCounter += 1;
			else if (ConfigFileContent[i][j] == '}')
				RightBracketCounter += 1;
		}
	}
	return (LeftBracketCounter == RightBracketCounter);
}

/*
 * Check if each non-context line has a semicolon (;)
 */

bool						Server::LastCharIsASemiColon(std::vector<std::string> &ConfigFileContent)	const
{
	size_t								j = 0;

	for (size_t i = 0; i < ConfigFileContent.size(); i += 1)
	{
		if (ConfigFileContent[i].find('{') == std::string::npos &&
				ConfigFileContent[i].find('}') == std::string::npos)
		{
			j = ConfigFileContent[i].size() - 1;
			while (j > 0 && isblank(ConfigFileContent[i][j]))
				j -= 1;

			if (ConfigFileContent[i][j] != ';')
				return (false);
		}
	}
	return (true);
}

/*
 * Basic error checking
 */

void						Server::ErrorChecker(std::vector<std::string> &ConfigFileContent)	const
{
	if (!OutOfServerContextCheck(ConfigFileContent)
			|| !CheckBrackets(ConfigFileContent)
			|| !LastCharIsASemiColon(ConfigFileContent))
		WriteErrorMessage("Error found when parsing file");
}

/*
 * Removes the semicolons (;) in the file
 */

void						Server::SemiColonRemover(std::vector<std::string> &ConfigFileContent)
{
	for (size_t i = 0; i < ConfigFileContent.size(); i += 1)
	{
		for (size_t j = 0; j < ConfigFileContent[i].size(); j += 1)
		{
			if (ConfigFileContent[i][j] == ';')
				ConfigFileContent[i].erase(j);
		}
	}
}

/*
 * Assembles the strings in the scope of a location tag in the configuration file together so
 * they form one string
 *
 * BEFORE:
 *         location /post {
 *                       limit_except POST;
 *                       client_max_body_size 100;
 *			}
 *
 *	AFTER:
 *		location /post {limit_except POST;client_max_body_size 100;}
 */

void						Server::LocationScopeAssembler(std::vector<std::string> &ConfigFileContent)
{
	size_t											LocationTagLine = 0;

	for (size_t i = 0; i < ConfigFileContent.size(); i += 1)
	{
		if (ConfigFileContent[i].rfind("location", 0) == 0)
		{
			LocationTagLine = i;
			i += 1;
			while (i != ReturnEndOfScopeLineNumber(LocationTagLine, ConfigFileContent))
			{
				ConfigFileContent[LocationTagLine].append(ConfigFileContent[i] + ';');
				ConfigFileContent.erase(ConfigFileContent.begin() + i);
			}
			ConfigFileContent[LocationTagLine].append(ConfigFileContent[i]);
			ConfigFileContent.erase(ConfigFileContent.begin() + i);
			i -= 1;
		}
	}
}

/*
 * Finds all location scopes in the config's provided content
 */

void						Server::LocationFinder(std::vector<std::string> &ConfigFileContent)
{
	for (size_t i = 0; i < ConfigFileContent.size(); i += 1)
	{
		if (ConfigFileContent[i].rfind("location /", 0) != std::string::npos)
			LocationSaver(ConfigFileContent[i]);
	}
}

/*
 * Adds a location object to the _Locations vector
 */

void						Server::LocationSaver(std::string LocationContent)
{
	Location				NewLocation(LocationContent.substr(9, std::string::npos));

	_Locations.push_back(NewLocation);
}

/*
 * Basic error check to see if the port number is not invalid
 */

void						Server::CheckIfValidPorts(std::string Ports)
{
	for (size_t i = 0; i < Ports.size(); i += 1)
	{
		if (!(std::isdigit(Ports[i])) && !(isblank(Ports[i])))
			WriteErrorMessage("Invalid listen ports");
	}
}

/*
 * Finds the listen ports and saves them
 */

void						Server::FindListenPorts(std::vector<std::string> &ConfigFileContent)
{
	size_t					EndOfPort;
	std::string				Ports;

	for (size_t i = 0; i < ConfigFileContent.size(); i += 1)
	{
		if (ConfigFileContent[i].rfind("listen", 0) == 0)
		{
			Ports = ConfigFileContent[i].substr(ConfigFileContent[i].find_first_of(' ', 0) + 1, ConfigFileContent[i].size());
			CheckIfValidPorts(Ports);
			if ((EndOfPort = Ports.find_first_of(' ', 0)) == std::string::npos)
				_ListenPorts.push_back(size_t(stoi(Ports)));
			else
			{
				for (size_t j = 0; j < Ports.size(); j += 1)
				{
					if ((EndOfPort = Ports.find_first_of(' ', j)) != std::string::npos)
					{
						_ListenPorts.push_back(size_t(stoi(Ports.substr(j, EndOfPort - j))));
						j = EndOfPort;
					}
					else
					{
						_ListenPorts.push_back(size_t(stoi(Ports.substr(j, Ports.size() - j))));
						break ;
					}
				}
			}
		}
	}
}

/*
 * Finds the server_name key and value and saves it
 */

void						Server::FindServerNames(std::vector<std::string> &ConfigFileContent)
{
	size_t					EndOfName;
	std::string				Names;

	for (size_t i = 0; i < ConfigFileContent.size(); i += 1)
	{
		if (ConfigFileContent[i].rfind("server_name", 0) == 0)
		{
			Names = ConfigFileContent[i].substr(ConfigFileContent[i].find_first_of(' ', 0) + 1, ConfigFileContent[i].size());
			if ((EndOfName = Names.find_first_of(' ', 0)) == std::string::npos)
				_ServerNames.push_back(Names);
			else
			{
				for (size_t j = 0; j < Names.size(); j += 1)
				{
					if ((EndOfName = Names.find_first_of(' ', j)) != std::string::npos)
					{
						_ServerNames.push_back(Names.substr(j, EndOfName - j));
						j = EndOfName;
					}
					else
					{
						_ServerNames.push_back(Names.substr(j, Names.size() - j));
						break ;
					}
				}
			}
			return ;
		}
	}
}

/*
 * Adds data sitting outside of location scopes to each location scope
 */

void						Server::AddOutsideInfoToLocation(std::vector<std::string> &ConfigFileContent)
{
	std::string				OutOfLocationContent;

	for (size_t i = 0; i < ConfigFileContent.size(); i += 1)
	{
		if (!ConfigFileContent[i].rfind("listen", 0) || !ConfigFileContent[i].rfind("server_name", 0)
				|| !ConfigFileContent[i].rfind("server", 0) || !ConfigFileContent[i].rfind("location", 0)
				|| !ConfigFileContent[i].rfind("}", 0))
			continue ;
		else
			OutOfLocationContent += ConfigFileContent[i] + ';';
	}

	for (size_t i = 0; i < ConfigFileContent.size(); i += 1)
	{
		if (ConfigFileContent[i].rfind("location", 0) == 0)
		{
			ConfigFileContent[i].resize(ConfigFileContent[i].size() - 1);
			ConfigFileContent[i] += OutOfLocationContent + '}';
		}
	}
}
