/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigReader.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesalle <mdesalle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 11:36:14 by mdesalle          #+#    #+#             */
/*   Updated: 2022/01/20 15:37:39 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigReader.hpp"

//======================== CONSTRUCTORS / DESTRUCTORS ========================//

/*
 * Save the config file's content in a vector of strings with each line in
 * the file being a string
 */

ConfigReader::ConfigReader(char *file_name)
{
	std::string		line;
	std::ifstream	file(file_name);

	if (!file.is_open())
		WriteErrorMessage("File could not be opened");

	while (std::getline(file, line))
	{
		if (!line.empty() && !FirstCharIsAHashtag(line))
			_ConfigFileContent.push_back(line);
	}

	file.close();
	CommentEraser();
	SpaceEraser();
	if (!CheckBrackets() || !LastCharIsASemiColon())
		WriteErrorMessage("Error found when parsing file");
	ServerSaver();
}

ConfigReader::ConfigReader(ConfigReader const &ref)
{
	*this = ref;
}

ConfigReader::~ConfigReader(void)
{
}

//================================ OPERATORS =================================//

ConfigReader							&ConfigReader::operator=(ConfigReader const &ref)
{
	this->_ConfigFileContent = ref.GetConfigFileContent();
	return (*this);
}

//============================ GETTERS / SETTERS =============================//


std::vector<std::string>				ConfigReader::GetConfigFileContent(void)	const
{
	return (_ConfigFileContent);
}

std::vector<std::vector<std::string> >	ConfigReader::GetServers(void)	const
{
	return (_Servers);
}

//================================ FUNCTIONS =================================//

void									ConfigReader::WriteErrorMessage(std::string message)	const
{
	std::cerr << message << std::endl;
	exit(1);
}

/*
 * Check if the whole line is a comment (which start with a hashtag)
 */

bool									ConfigReader::FirstCharIsAHashtag(std::string line) const
{
	size_t								i = 0;

	while (i < line.size() && std::isblank(line[i]))
		i += 1;
	return (line[i] == '#');
}


/*
 * Check if each bracket left bracket { is matched with a right bracket }
 */

bool									ConfigReader::CheckBrackets(void)	const
{
	size_t								LeftBracketCounter = 0;
	size_t								RightBracketCounter = 0;

	for (size_t i = 0; i < GetConfigFileContent().size(); i += 1)
	{
		for (size_t j = 0; j < GetConfigFileContent()[i].size(); j += 1)
		{
			if (GetConfigFileContent()[i][j] == '{')
				LeftBracketCounter += 1;
			else if (GetConfigFileContent()[i][j] == '}')
				RightBracketCounter += 1;
		}
	}
	return (LeftBracketCounter == RightBracketCounter);
}

/*
 * Check if each non-context line has a semicolon (;)
 */

bool									ConfigReader::LastCharIsASemiColon(void)	const
{
	size_t								j = 0;

	for (size_t i = 0; i < GetConfigFileContent().size(); i += 1)
	{
		if (GetConfigFileContent()[i].find('{') == std::string::npos &&
				GetConfigFileContent()[i].find('}') == std::string::npos)
		{
			j = GetConfigFileContent()[i].size() - 1;
			while (j > 0 && isblank(GetConfigFileContent()[i][j]))
				j -= 1;

			if (GetConfigFileContent()[i][j] != ';')
				return (false);
		}
	}
	return (true);
}

void									ConfigReader::SpaceEraser(void)
{
	size_t								j = 0;

	for (size_t i = 0; i < GetConfigFileContent().size(); i += 1)
	{
		while (j < GetConfigFileContent()[i].size() && std::isblank(GetConfigFileContent()[i][j]))
			j += 1;
		_ConfigFileContent[i].erase(0, j);
		j = 0;
	}
}

/*
 * Remove all comments from config file
 */

void									ConfigReader::CommentEraser(void)
{
	for (size_t i = 0; i < GetConfigFileContent().size(); i += 1)
	{
		for (size_t j = 0; j < GetConfigFileContent()[i].size(); j += 1)
		{
			if (GetConfigFileContent()[i][j] == '#')
			{
				_ConfigFileContent[i].erase(j, GetConfigFileContent()[i].size());
				break ;
			}
		}
	}
}

size_t									ConfigReader::ReturnEndOfScopeLineNumber(size_t BeginBracketsLineNumber)	const
{
	size_t								NumberOfOpenScopes = 1;

	for (size_t i = BeginBracketsLineNumber + 1; i < GetConfigFileContent().size(); i += 1)
	{
		for (size_t j = 0; j < GetConfigFileContent()[i].size(); j += 1)
		{
			if (GetConfigFileContent()[i][j] == '{')
				NumberOfOpenScopes += 1;
			else if (GetConfigFileContent()[i][j] == '}')
				NumberOfOpenScopes -= 1;

			if (NumberOfOpenScopes == 0)
				return (i);
		}
	}
	return (0);
}

/*
 * Creates a vector for each server configuration found in the config file
 */

void									ConfigReader::ServerSaver(void)
{
	size_t								ScopeSize = 0;
	std::vector<std::string>			ServerScope;

	for (size_t i = 0; i < GetConfigFileContent().size(); i += 1)
	{
		if (!GetConfigFileContent()[i].rfind("server {", 0))
		{
			ScopeSize = ReturnEndOfScopeLineNumber(i);
			for (i += 1; i < ScopeSize; i += 1)
				ServerScope.push_back(GetConfigFileContent()[i]);
			_Servers.push_back(ServerScope);
			ServerScope.clear();
		}
	}
}
