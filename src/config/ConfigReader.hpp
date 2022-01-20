/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigReader.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesalle <mdesalle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 11:38:06 by mdesalle          #+#    #+#             */
/*   Updated: 2022/01/20 15:36:01 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGREADER_HPP
# define CONFIGREADER_HPP

#include "Config.hpp"

class ConfigReader
{
	public:

		ConfigReader(char *file_name);
		ConfigReader(ConfigReader const &ref);
		~ConfigReader(void);
		ConfigReader	&operator=(ConfigReader const &ref);

		std::vector<std::string>				GetConfigFileContent(void)									const;
		std::vector<std::vector<std::string> >	GetServers(void)											const;

		void									WriteErrorMessage(std::string message)						const;
		bool									FirstCharIsAHashtag(std::string line)						const;
		void									CommentEraser(void);
		bool									CheckBrackets(void)											const;
		bool									LastCharIsASemiColon(void)									const;
		size_t									ReturnEndOfScopeLineNumber(size_t BeginBracketsLineNumber)	const;
		void									ServerSaver(void);
		void									SpaceEraser(void);

	private:

		std::vector<std::string>	_ConfigFileContent;
		std::vector<std::vector<std::string> >	_Servers;
};

#endif
