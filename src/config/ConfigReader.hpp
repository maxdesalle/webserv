/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigReader.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesalle <mdesalle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 11:38:06 by mdesalle          #+#    #+#             */
/*   Updated: 2022/01/19 16:51:36 by mdesalle         ###   ########.fr       */
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

		std::vector<std::string>	GetConfigFileContent(void)				const;

		bool						CheckBrackets(void)						const;
		bool						CheckSemiColon(void)					const;
		bool						FirstCharIsAHashtag(std::string line)	const;

	private:

		std::vector<std::string>	_ConfigFileContent;
};

#endif
