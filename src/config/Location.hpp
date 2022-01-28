/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesalle <mdesalle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 13:20:37 by mdesalle          #+#    #+#             */
/*   Updated: 2022/01/28 14:19:50 by mdesalle         ###   ########.fr       */
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
		Location	&operator=(Location const &ref);

		std::string	GetCGI(void)				const;
		std::string	GetPath(void)				const;
		std::string	GetRoot(void)				const;
		std::string	GetIndex(void)				const;
		std::string	GetAutoIndex(void)			const;
		std::string	GetErrorPage(void)			const;
		std::string	GetReturnCode(void)			const;
		std::string	GetLimitExcept(void)		const;
		std::string	GetAllowMethod(void)		const;
		std::string	GetClientMaxBodySize(void)	const;

	private:

		std::string	_CGI;
		std::string	_Path;
		std::string	_Root;
		std::string	_Index;
		std::string	_AutoIndex;
		std::string	_ErrorPage;
		std::string	_Return;
		std::string	_LimitExcept;
		std::string	_AllowMethod;
		std::string	_ClientMaxBodySize;
};

#endif
