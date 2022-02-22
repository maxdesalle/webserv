/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:37:00 by mdesalle          #+#    #+#             */
/*   Updated: 2022/02/22 17:13:01 by tderwedu         ###   ########.fr       */
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
		Server	&operator=(Server const &ref);

		std::vector<Location> const								&GetLocations(void)			const;
		std::vector<std::string> const							GetServerNames(void)		const;
		std::map<std::string, std::vector<size_t> > const&		GetDefaultServer(void)		const;
		std::map<std::string, std::vector<size_t> > const&		GetListenIPandPorts(void)	const;

		bool			CheckBrackets(std::vector<std::string> &ConfigFileContent)				const;
		bool			LastCharIsASemiColon(std::vector<std::string> &ConfigFileContent)		const;
		bool			OutOfServerContextCheck(std::vector<std::string> &ConfigFileContent)	const;
		void			WriteErrorMessage(std::string message)									const;
		void			ErrorChecker(std::vector<std::string> &ConfigFileContent)				const;
		void			CheckIfValidPorts(std::string Ports);
		void			LocationSaver(std::string LocationContent);
		void			InsertNewPort(std::string IP, std::string Ports);
		void			AddListenInfo(std::string ConfigLine, bool DefaultServer);
		void			InsertNewDefaultServer(std::string IP, std::string Ports);
		void			CommentEraser(std::vector<std::string> &ConfigFileContent);
		void			LocationFinder(std::vector<std::string> &ConfigFileContent);
		void			FindServerNames(std::vector<std::string> &ConfigFileContent);
		void			FrontSpaceEraser(std::vector<std::string> &ConfigFileContent);
		void			SemiColonRemover(std::vector<std::string> &ConfigFileContent);
		void			FindListenIPandPorts(std::vector<std::string> &ConfigFileContent);
		void			LocationScopeAssembler(std::vector<std::string> &ConfigFileContent);
		void			AddOutsideInfoToLocation(std::vector<std::string> &ConfigFileContent);
		void			IPWithPort(std::string ConfigLine, size_t EndOfIP, bool DefaultServer);
		void			SinglePort(std::string Ports, bool DefaultServer);
		void			MultiplePorts(std::string Ports, bool DefaultServer);
		size_t			ReturnEndOfScopeLineNumber(size_t BeginBracketsLineNumber, std::vector<std::string> &ConfigFileContent)	const;
		std::string		FindPortsInString(std::string ConfigLine);
		void			printServer(void) const;

	private:

		std::vector<Location>							_Locations;
		std::vector<std::string>						_ServerNames;
		std::map<std::string, std::vector<size_t> >		_DefaultServer;
		std::map<std::string, std::vector<size_t> >		_ListenIPandPorts;
};

#endif
