/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 14:57:25 by ldelmas           #+#    #+#             */
/*   Updated: 2022/01/26 15:53:47 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"

/*CONSTRUCTORS AND DESTRUCTORS*/

Header::Header(void) {}

Header::Header(std::string const *fieldNames)
{
	for (int i = 0; fieldNames[i][0]; i++)
		this->headerFields.insert(std::pair<std::string const, std::string>(fieldNames[i], ""));
}

Header::Header(Header const &src) {this->headerFields = src.headerFields;}

Header::~Header(void) {}


/*OPERATOR OVERLOADS*/

Header const &Header::operator=(Header const &right)
{
	this->headerFields = right.headerFields;
	return *this;
}


/*GETTERS AND SETTERS*/

std::string const	Header::getField(std::string const fieldName)
{
	if (this->headerFields.find(fieldName) == this->headerFields.end())
		return ("");
	return this->headerFields[fieldName];
}

void				Header::setField(std::string const fieldName, std::string fieldValue)
{
	if (this->headerFields.find(fieldName) == this->headerFields.end())
		return ;
	this->headerFields[fieldName] = fieldValue;
}