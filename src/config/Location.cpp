/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesalle <mdesalle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 14:30:39 by mdesalle          #+#    #+#             */
/*   Updated: 2022/01/28 14:40:03 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

//======================== CONSTRUCTORS / DESTRUCTORS ========================//

Location::Location(std::string LocationContent)
{
}

Location::Location(Location const &ref)
{
	*this = ref;
}

Location::~Location(void)
{
}

//================================ OPERATORS =================================//

Location	&Location::operator=(Location const &ref)
{
	this->_CGI = ref.GetCGI();
	this->_Path = ref.GetPath();
	this->_Root = ref.GetRoot();
	this->_Index = ref.GetIndex();
	this->_AutoIndex = ref.GetAutoIndex();
	this->_ErrorPage = ref.GetErrorPage();
	this->_Return = ref.GetReturn();
	this->_LimitExcept = ref.GetLimitExcept();
	this->_AllowMethod = ref.GetAllowMethod();
	this->_ClientMaxBodySize = ref.GetClientMaxBodySize();
	return (*this);
}

//============================ GETTERS / SETTERS =============================//

std::string	Location::GetCGI(void)					const
{
	return (_CGI);
}

std::string	Location::GetPath(void)					const
{
	return (_Path);
}

std::string	Location::GetRoot(void)					const
{
	return (_Root);
}

std::string	Location::GetIndex(void)				const
{
	return (_Index);
}

std::string	Location::GetAutoIndex(void)			const
{
	return (_AutoIndex);
}

std::string	Location::GetErrorPage(void)			const
{
	return (_ErrorPage);
}

std::string	Location::GetReturn(void)				const
{
	return (_Return);
}

std::string	Location::GetLimitExcept(void)			const
{
	return (_LimitExcept);
}

std::string	Location::GetAllowMethod(void)			const
{
	return (_AllowMethod);
}

std::string	Location::GetClientMaxBodySize(void)	const
{
	return (_ClientMaxBodySize);
}

//================================ FUNCTIONS =================================//


