/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigReader.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesalle <mdesalle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 11:36:14 by mdesalle          #+#    #+#             */
/*   Updated: 2022/01/19 16:51:53 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigReader.hpp"

/*
 * Save the config file's content in a vector of strings with each line in
 * the file being a string
 */

ConfigReader::ConfigReader(char *file_name)
{
	std::ifstream	file(file_name);

	if (file.is_open())
	{
		std::string	line;
		while (std::getline(file, line))
		{
			if (!line.empty() && !FirstCharIsAHashtag(line))
				_ConfigFileContent.push_back(line);
		}
		file.close();
	}
	else
	{
		std::cerr << "File could not be opened" << std::endl;
		exit(1);
	}
	if (!CheckBrackets() || !CheckSemiColon())
	{
		std::cerr << "Error found when parsing file" << std::endl;
		exit(1);
	}
}

ConfigReader::ConfigReader(ConfigReader const &ref)
{
	*this = ref;
}

ConfigReader::~ConfigReader(void)
{
}

ConfigReader				&ConfigReader::operator=(ConfigReader const &ref)
{
	this->_ConfigFileContent = ref.GetConfigFileContent();
	return (*this);
}

std::vector<std::string>	ConfigReader::GetConfigFileContent(void)	const
{
	return (_ConfigFileContent);
}

/*
 * Check if the whole line is a comment (which start with a hashtag)
 */

bool						ConfigReader::FirstCharIsAHashtag(std::string line) const
{
	size_t	i = 0;

	while (i < line.size() && std::isblank(line[i]))
		i += 1;
	return (line[i] == '#');
}

/*
 * Check if each bracket left bracket { is matched with a right bracket }
 */

bool						ConfigReader::CheckBrackets(void)			const
{
	size_t					LeftBracketCounter = 0;
	size_t					RightBracketCounter = 0;

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
 * Check if each non-context line has a semicolon at end
 */

bool						ConfigReader::CheckSemiColon(void)			const
{
	for (size_t i = 0; i < GetConfigFileContent().size(); i += 1)
	{
		if (GetConfigFileContent()[i].find(';') == std::string::npos)
		{
			if (GetConfigFileContent()[i].find('{') == std::string::npos &&
					GetConfigFileContent()[i].find('}') == std::string::npos &&
					GetConfigFileContent()[i + 1].find('{') == std::string::npos &&
					GetConfigFileContent()[i + 1].find('}') == std::string::npos)
			{
				std::cout << GetConfigFileContent()[i] << std::endl;
				return (false);
			}
		}
	}
	return (true);
}
