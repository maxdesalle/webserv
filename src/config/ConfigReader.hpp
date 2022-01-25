/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigReader.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesalle <mdesalle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 11:38:06 by mdesalle          #+#    #+#             */
/*   Updated: 2022/01/25 17:42:11 by mdesalle         ###   ########.fr       */
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

		std::vector<std::string>							GetConfigFileContent(void)										const;
		std::vector<std::map<std::string, std::string> >	GetServers(void)												const;
		std::vector<std::string>							GetLocations(size_t RequestedServerNumber)						const;
		std::map<std::string, std::string>					GetLocationContent(size_t ServerNumber, std::string Location)	const;

		void												WriteErrorMessage(std::string message)							const;
		bool												FirstCharIsAHashtag(std::string line)							const;
		bool												CheckBrackets(void)												const;
		bool												LastCharIsASemiColon(void)										const;
		size_t												ReturnEndOfScopeLineNumber(size_t BeginBracketsLineNumber)		const;
		void												ServerSaver(void);
		void												CommentEraser(void);
		void												FrontSpaceEraser(void);
		void												SemiColonRemover(void);
		void												LocationScopeAssembler(void);
		bool												FindPrefix(std::string Prefix, size_t LineNumber)				const;
		std::pair<std::string, std::string>					PairMaker(std::string KeyValue)									const;

	private:

		std::vector<std::string>							_ConfigFileContent;
		std::vector<std::map<std::string, std::string> >	_Servers;
};

#endif
