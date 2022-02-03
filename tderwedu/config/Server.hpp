/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesalle <mdesalle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:37:00 by mdesalle          #+#    #+#             */
/*   Updated: 2022/02/02 18:47:10 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "Config.hpp"

class Location;

class Server
{
	public:

		Server(std::vector<std::string> ConfigFileContent);
		Server(Server const &ref);
		~Server(void);
		Server						&operator=(Server const &ref);

		std::vector<size_t>			GetListenPorts(void)	const;
		std::vector<std::string>	GetServerNames(void)	const;
		std::vector<Location>		GetLocations(void)		const;

		void						ErrorChecker(std::vector<std::string> &ConfigFileContent)	const;
		bool						CheckBrackets(std::vector<std::string> &ConfigFileContent)	const;
		bool						LastCharIsASemiColon(std::vector<std::string> &ConfigFileContent)	const;
		void						CommentEraser(std::vector<std::string> &ConfigFileContent);
		void						FrontSpaceEraser(std::vector<std::string> &ConfigFileContent);
		void						SemiColonRemover(std::vector<std::string> &ConfigFileContent);
		void						LocationScopeAssembler(std::vector<std::string> &ConfigFileContent);
		bool						OutOfServerContextCheck(std::vector<std::string> &ConfigFileContent)	const;
		void						WriteErrorMessage(std::string message)	const;
		void						LocationSaver(std::string LocationContent);
		void						CheckIfValidPorts(std::string Ports);
		void						FindListenPorts(std::vector<std::string> &ConfigFileContent);
		void						FindServerNames(std::vector<std::string> &ConfigFileContent);
		void						AddOutsideInfoToLocation(std::vector<std::string> &ConfigFileContent);
		void						LocationFinder(std::vector<std::string> &ConfigFileContent);
		size_t						ReturnEndOfScopeLineNumber(size_t BeginBracketsLineNumber, std::vector<std::string> &ConfigFileContent)	const;

	private:

		std::vector<size_t>			_ListenPorts;
		std::vector<std::string>	_ServerNames;
		std::vector<Location>		_Locations;
};

#endif
