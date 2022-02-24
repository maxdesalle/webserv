/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 14:58:24 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/24 17:08:05 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::string const Response::_fieldNames[16] = {"Age", "Cache-Control", "Expires", "Date", "Location", "Retry-After", "Vary", "Warning", "ETag", "Last-Modified", "WWW-Authenticate", "Proxy-Authenticate", "Accept-Ranges", "Allow", "Server", ""};

Response::Response(void) : Header(Response::_fieldNames)
{
}

Response::Response(Response const &src)
{
	this->_headerFields = src._headerFields;
}

Response::~Response(void)
{
}

Response const &Response::operator=(Response const &ref)
{
	this->_headerFields = ref._headerFields;
	return *this;
}

std::string	Response::GetServerVersion(void)
{
	return ("WEBSERV/1.0");
}

std::string	Response::GetHeaderResponse(Request HTTPRequest, std::string Path)
{
	_HeaderResponse = HTTPRequest.getVersion() + " " + <<<!!! status code !!!>>> + <<<!!! OK or not found !!!>>>; // to add later
	_HeaderResponse += "Date: " + GetCurrentFormattedTime() + "\n";
	_HeaderResponse += "Server: " + GetServerVersion() + "\n";
	_HeaderResponse += "Last-Modified: " + GetLastModifiedTimeForFile(Path) + "\n";
	_HeaderResponse += "Content-Lenght: " + this->_headerFields["Content-Length"] + "\n";
	_HeaderResponse += "Content-Type: " + this->_headerFields["Content-Type"] + "\n";
	_HeaderResponse += "Connection: " + this->_headerFields["Connection"] + "\n\n";
	_HeaderResponse += HTTPRequest.getBody(); // is this right? not sure...
}

std::string	Response::GetCurrentFormattedTime(void)
{
	time_t		t = time(NULL);
	struct		tm *tm = gmtime(&t);
	std::string	FormattedString;

	FormattedString.resize(29);
	strftime(&FormattedString[0], FormattedString.size(), "%a, %d %b %Y %X GMT", tm);

	return (FormattedString);
}

std::string	Response::GetLastModifiedTimeForFile(std::string Path)
{
	struct		stat attr;
	struct		tm *tm = gmtime(stat(Path, &attr));
	std::string	FormattedString;

	FormattedString.resize(29);
	strftime(&FormattedString[0], FormattedString.size(), "%a, %d %b %Y %X GMT", tm);

	return (FormattedString);
}
