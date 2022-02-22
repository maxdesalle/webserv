/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 15:28:58 by mdesalle          #+#    #+#             */
/*   Updated: 2022/02/22 15:28:37 by tderwedu         ###   ########.fr       */
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
		std::string						GetPath(void)				const;
		std::string						GetPass(void)				const;
		std::string						GetRoot(void)				const;
		std::vector<std::string>		GetIndex(void)				const;
		std::vector<std::string>		GetLimitExcept(void)		const;
		std::map<size_t, std::string>	GetErrorPage(void)			const;
		std::map<size_t, std::string>	GetReturn(void)				const;

		bool							SubStringCompare(std::string KeyValue);
		void							SaveLimitExcept(std::string Value);
		void							SaveIndex(std::string Value);
		void							SaveReturn(std::string Value);
		void							SaveErrorPage(std::string Value);
		void							AddToLocationVector(std::string KeyValue);
		void							Assignator(std::string Key, std::string Value);
		size_t							FindPath(std::string LocationContent);
		std::string						FindKeyValue(std::string LocationContent, size_t &End);

		bool							isCgi(void)									const;
		bool							isMethodValid(const std::string &method)	const;
		void							printLocation(void) const;

	private:

		bool							_AutoIndex;
		size_t							_ClientMaxBodySize;
		std::string						_Path;
		std::string						_Root;
		std::string						_Pass;
		std::vector<std::string>		_Index;
		std::vector<std::string>		_LimitExcept;
		std::map<size_t, std::string>	_Return;
		std::map<size_t, std::string>	_ErrorPage;
};

#endif
