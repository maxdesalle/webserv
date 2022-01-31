/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 14:57:25 by ldelmas           #+#    #+#             */
/*   Updated: 2022/01/31 16:15:53 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"

/*CONSTRUCTORS AND DESTRUCTORS*/

std::string const	Header::_alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
std::string const	Header::_digit = "0123456789";
std::string const	Header::_unreserved = Header::_alpha + Header::_digit + "-._~";
std::string const	Header::_sub_delims = "!$&'()*+,;=";
std::string const	Header::_hexdig = Header::_digit + "ABCDEF";
std::string const	Header::_custom_pchar = Header::_unreserved + Header::_sub_delims + ":@";
std::string const 	Header::_authorizedMethods[4] = {"GET", "POST", "DELETE", ""};


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


/*NON MEMBER FUNCTIONS*/

std::string			Header::_parseHTTPVersion(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (s.substr(5) != "HTTP/" || Header::_digit.find(s[5])==std::string::npos
		|| s[6] != '.' || Header::_digit.find(s[7])==std::string::npos)
		return "";
	return s.substr(0, 8);
}

std::string			Header::_parseMethod(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	for (int i = 0; Header::_authorizedMethods[i]!=""; i++)
		if (Header::_authorizedMethods[i]==s.substr(Header::_authorizedMethods[i].length()))
			return Header::_authorizedMethods[i];
	return "";
}

std::string			Header::_parsePchar(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (Header::_custom_pchar.find(s[0]) != std::string::npos)
		return s.substr(0, 1);
	else if (s[0] == '%' && Header::_hexdig.find(s[1])!=std::string::npos
	&& Header::_hexdig.find(s[2])!=std::string::npos)
		return s.substr(0, 3);
	return "";
}

std::string			Header::_parseSegment(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string tmp = Header::_parsePchar(s);
	std::string segment = tmp;
	while (tmp != "")
	{
		tmp = Header::_parsePchar(s, segment.length());
		segment += tmp;
	}
	return segment;
}

std::string			Header::_parseQuery(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string tmp = Header::_parsePchar(s);
	if (tmp == "" && (s[0] == '/' || s[0] == '?'))
		tmp+=s[0];
	std::string query = tmp;
	while (tmp != "")
	{
		size_t len = query.length();
		tmp = Header::_parsePchar(s, len);
		if (tmp == "" && (s[len] == '/' || s[len] == '?'))
			tmp+=s[len];
		query += tmp;
	}
	return query;
}

std::string			Header::_parseAbsPath(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string path = "";
	while (1)
	{
		size_t len = path.length();
		if (s[len] != '/')
			return path;
		path += '/' + Header::_parseSegment(s, len+1);
	}
	return path;
}