/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigReader.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesalle <mdesalle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 11:36:14 by mdesalle          #+#    #+#             */
/*   Updated: 2022/01/26 17:18:43 by mdesalle         ###   ########.fr       */
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
	FrontSpaceEraser();
	ErrorChecker();
	SemiColonRemover();
	LocationScopeAssembler();
	ServerSaver();
	RequiredMapKeysCheck();
}

ConfigReader::ConfigReader(ConfigReader const &ref)
{
	*this = ref;
}

ConfigReader::~ConfigReader(void)
{
}

//================================ OPERATORS =================================//

ConfigReader										&ConfigReader::operator=(ConfigReader const &ref)
{
	this->_ConfigFileContent = ref.GetConfigFileContent();
	return (*this);
}

//============================ GETTERS / SETTERS =============================//


std::vector<std::string>							ConfigReader::GetConfigFileContent(void)	const
{
	return (_ConfigFileContent);
}

std::vector<std::map<std::string, std::string> >	ConfigReader::GetServers(void)	const
{
	return (_Servers);
}

std::vector<std::string>							ConfigReader::GetLocations(size_t RequestedServerNumber)	const
{
	size_t											i = 0;
	size_t											EndOfScope;
	size_t											CurrentServerNumber = 0;
	std::string										LocationName;
	std::vector<std::string>						Locations;

	while (CurrentServerNumber != RequestedServerNumber)
	{
		i = ReturnEndOfScopeLineNumber(i) + 1;
		CurrentServerNumber += 1;
	}
	EndOfScope = ReturnEndOfScopeLineNumber(i);
	for (; i < EndOfScope; i += 1)
	{
		if (!GetConfigFileContent()[i].rfind("location /", 0))
		{
			LocationName = GetConfigFileContent()[i].substr(10, GetConfigFileContent()[i].find_first_of('{') - 10);
			Locations.push_back(LocationName);
		}
	}
	return (Locations);
}

std::map<std::string, std::string>					ConfigReader::GetLocationContent(size_t ServerNumber, std::string Location)	const
{
	size_t											LastSemiColon = 0;
	std::string										KeyValue;
	std::map<std::string, std::string>				LocationGetContent;

	for (size_t i = 0; GetServers()[ServerNumber]["location /" + Location][i] != '}'; i += 1)
	{
		if (GetServers()[ServerNumber]["location /" + Location][i] == ';')
		{
			KeyValue = GetServers()[ServerNumber]["location /" + Location].substr(LastSemiColon + 1, i - LastSemiColon - 1);
			LocationGetContent.insert(PairMaker(KeyValue));
			KeyValue.clear();
			LastSemiColon = i;
		}
	}
	return (LocationGetContent);
}

//================================ FUNCTIONS =================================//

void												ConfigReader::ErrorChecker(void)	const
{
	if (!OutOfServerContextCheck() || !CheckBrackets() || !LastCharIsASemiColon())
		WriteErrorMessage("Error found when parsing file");
}

void												ConfigReader::RequiredMapKeysCheck(void)	const
{
	for (size_t i = 0; i < GetServers().size(); i += 1)
	{
		if (!GetServers()[i].count("listen") || !GetServers()[i].count("server_name"))
			WriteErrorMessage("Error when parsing file");
	}
}

void												ConfigReader::WriteErrorMessage(std::string message)	const
{
	std::cerr << message << std::endl;
	exit(1);
}

/*
 * Check if the whole line is a comment (which start with a hashtag)
 */

bool												ConfigReader::FirstCharIsAHashtag(std::string line) const
{
	size_t								i = 0;

	while (i < line.size() && std::isblank(line[i]))
		i += 1;
	return (line[i] == '#');
}


/*
 * Check if each bracket left bracket { is matched with a right bracket }
 */

bool												ConfigReader::CheckBrackets(void)	const
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

bool												ConfigReader::LastCharIsASemiColon(void)	const
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

/*
 * Removes all the unnecessary space int front of the config settings
 */

void												ConfigReader::FrontSpaceEraser(void)
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
 * Removes the semicolons (;) in the file
 */

void												ConfigReader::SemiColonRemover(void)
{
	for (size_t i = 0; i < GetConfigFileContent().size(); i += 1)
	{
		for (size_t j = 0; j < GetConfigFileContent()[i].size(); j += 1)
		{
			if (GetConfigFileContent()[i][j] == ';')
				_ConfigFileContent[i].erase(j);
		}
	}
}

/*
 * Remove all comments from config file
 */

void												ConfigReader::CommentEraser(void)
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

/*
 * Check if there are no configuration settings outside of a "server" context
 */

bool												ConfigReader::OutOfServerContextCheck(void)	const
{
	size_t											i = 0;

	if (GetConfigFileContent()[0].rfind("server {", 0))
		return (false);
	while (i < GetConfigFileContent().size() - 1)
	{
		i = ReturnEndOfScopeLineNumber(i) + 1;
		if (i != GetConfigFileContent().size())
		{
			if (GetConfigFileContent()[i].rfind("server {", 0))
				return (false);
		}
	}
	return (true);
}

/*
 * Returns the line number where the bracket located in the line number passed as argument
 * is closed (so the scope of the brackets is closed)
 */

size_t												ConfigReader::ReturnEndOfScopeLineNumber(size_t BeginBracketsLineNumber)	const
{
	size_t											NumberOfOpenScopes = 1;

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

void												ConfigReader::LocationScopeAssembler(void)
{
	size_t											LocationTagLine = 0;

	for (size_t i = 0; i < GetConfigFileContent().size(); i += 1)
	{
		if (GetConfigFileContent()[i].rfind("location", 0) == 0)
		{
			LocationTagLine = i;
			i += 1;
			while (i != ReturnEndOfScopeLineNumber(LocationTagLine))
			{
				_ConfigFileContent[LocationTagLine].append(GetConfigFileContent()[i] + ';');
				_ConfigFileContent.erase(_ConfigFileContent.begin() + i);
			}
			_ConfigFileContent[LocationTagLine].append(GetConfigFileContent()[i]);
			_ConfigFileContent.erase(_ConfigFileContent.begin() + i);
			i -= 1;
		}
	}
}

/*
 * Returns true if the string in the vector starts with the given prefix.
 */

bool												ConfigReader::FindPrefix(std::string Prefix, size_t LineNumber)	const
{
	return (GetConfigFileContent()[LineNumber].rfind(Prefix, 0) == 0);
}

/*
 * Creates a pair based on a string where a space separates the key from the value
 */

std::pair<std::string, std::string>					ConfigReader::PairMaker(std::string KeyValue)	const
{
	size_t											FirstSpaceInString = 0;

	FirstSpaceInString = KeyValue.find_first_of(' ');
	if (KeyValue.rfind("return", 0) == 0 || KeyValue.rfind("error_page", 0) == 0)
		FirstSpaceInString = KeyValue.find(' ', FirstSpaceInString + 1);
	return (make_pair(KeyValue.substr(0, FirstSpaceInString), KeyValue.substr(FirstSpaceInString + 1, KeyValue.size())));
}

/*
 * Creates a vector for each server configuration found in the config file
 */

void												ConfigReader::ServerSaver(void)
{
	size_t											ScopeSize = 0;
	size_t											FirstSpaceInString = 0;
	std::map<std::string, std::string>				ServerDictionary;

	for (size_t i = 0; i < GetConfigFileContent().size(); i += 1)
	{
		if (!GetConfigFileContent()[i].rfind("server {", 0))
		{
			ScopeSize = ReturnEndOfScopeLineNumber(i);
			for (i += 1; i < ScopeSize; i += 1)
			{
				if ((FirstSpaceInString = GetConfigFileContent()[i].find_first_of(' ')) != std::string::npos)
				{
					if (FindPrefix("location", i) || FindPrefix("return", i) || FindPrefix("error_page", i))
						FirstSpaceInString = GetConfigFileContent()[i].find(' ', FirstSpaceInString + 1);
					ServerDictionary.insert(make_pair(GetConfigFileContent()[i].substr(0, FirstSpaceInString), GetConfigFileContent()[i].substr(FirstSpaceInString + 1, GetConfigFileContent()[i].size())));
				}
			}
			_Servers.push_back(ServerDictionary);
			ServerDictionary.clear();
		}
	}
}
