/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesalle <mdesalle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:20:37 by mdesalle          #+#    #+#             */
/*   Updated: 2022/02/16 13:47:27 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "Config.hpp"

class Location
{
	public:

		Location(std::string LocationContent);
		Location(Location const &ref);
		~Location(void);
		Location						&operator=(Location const &ref);

		bool							GetAutoIndex(void)			const;
		size_t							GetClientMaxBodySize(void)	const;
		std::string						GetCGI(void)				const;
		std::string						GetPath(void)				const;
		std::string						GetPass(void)				const;
		std::string						GetRoot(void)				const;
		std::vector<std::string>		GetIndex(void)				const;
		std::vector<std::string>		GetLimitExcept(void)		const;
		std::vector<std::string>		GetAllowMethod(void)		const;
		std::map<size_t, std::string>	GetErrorPage(void)			const;
		std::map<size_t, std::string>	GetReturn(void)				const;

		bool							SubStringCompare(std::string KeyValue);
		void							SaveLimitExcept(std::string Value);
		void							SaveAllowMethod(std::string Value);
		void							SaveIndex(std::string Value);
		void							SaveReturn(std::string Value);
		void							SaveErrorPage(std::string Value);
		void							AddToLocationVector(std::string KeyValue);
		void							Assignator(std::string Key, std::string Value);
		size_t							FindPath(std::string LocationContent);
		std::string						FindKeyValue(std::string LocationContent, size_t &End);

	private:

		bool							_AutoIndex;
		size_t							_ClientMaxBodySize;
		std::string						_CGI;
		std::string						_Path;
		std::string						_Root;
		std::string						_Pass;
		std::vector<std::string>		_Index;
		std::vector<std::string>		_LimitExcept;
		std::vector<std::string>		_AllowMethod;
		std::map<size_t, std::string>	_Return;
		std::map<size_t, std::string>	_ErrorPage;
};

#endif
