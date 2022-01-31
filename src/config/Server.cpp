/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesalle <mdesalle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 14:07:11 by mdesalle          #+#    #+#             */
/*   Updated: 2022/01/28 17:34:52 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//======================== CONSTRUCTORS / DESTRUCTORS ========================//

Server::Server(std::string ConfigFileName)
{
	std::string					Line;
	std::ifstream				File(ConfigFileName);
	std::vector<std::string>	ConfigFileContent;

	if (!File.is_open())
		WriteErrorMessage("File could not be opened");

	while (std::getline(File, Line))
	{
		if (!Line.empty() && !FirstCharIsAHashtag(Line))
			ConfigFileContent.push_back(Line);
	}

	File.close();
	CommentEraser(ConfigFileContent);
	FrontSpaceEraser(ConfigFileContent);
	ErrorChecker(ConfigFileContent);
	SemiColonRemover(ConfigFileContent);
	LocationScopeAssembler(ConfigFileContent);
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
	/* this->_Locations = ref.GetLocations(); */
	return (*this);
}

//============================ GETTERS / SETTERS =============================//

std::vector<int>			Server::GetListenPorts(void)	const
{
	return (_ListenPorts);
}

std::vector<std::string>	Server::GetServerNames(void)	const
{
	return (_ServerNames);
}

/* std::vector<Locations>		Server::GetLocations(void)		const */
/* { */
/* 	return (_Locations); */
/* } */

//================================ FUNCTIONS =================================//

/*
 * Check if the whole line is a comment (which start with a hashtag)
 */

bool						Server::FirstCharIsAHashtag(std::string line)	const
{
	size_t								i = 0;

	while (i < line.size() && std::isblank(line[i]))
		i += 1;
	return (line[i] == '#');
}

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
