/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 14:59:17 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/02 17:11:02 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Request.hpp"

/* Important static attributes*/

std::string const Request::_fieldNames[22] = {"Cache-Control", "Expect", "Host", 
										"Max-Forwards", "Pragma", "Range",
										"TE", "If-Match", "If-None-Match",
										"If-Modified-Since", "If-Unmodified-Since",
										"If-Range", "Accept", "Accept-Charset",
										"Accept-Encoding", "Accept-Language",
										"Authorization", "Proxy-Authorization",
										"From", "Referer", "User-Agent", ""};



/*CONSTRUCTORS AND DESTRUCTORS*/

Request::Request(void) : Header(Request::_fieldNames) {}

Request::Request(Request const &src) {this->_headerFields = src._headerFields;}

Request::~Request(void) {}



/*OPERATOR OVERLOADS*/

Request const	&Request::operator=(Request const &right)
{
	this->_headerFields = right._headerFields;
	return *this;
}



/*GETTERS AND SETTERS*/

std::string const	&Request::getMethod(void) const
{
	return this->_method;
}

std::string const	&Request::getTarget(void) const
{
	return this->_target;
}

std::string const	&Request::getVersion(void) const
{
	return this->_version;
}

std::string const	&Request::getBody(void) const
{
	return this->_body;
}

void				Request::setMethod(std::string &method)
{
	this->_method = method;
}

void				Request::setTarget(std::string &target)
{
	this->_target = target;
}

void				Request::setVersion(std::string &version)
{
	this->_version = version;
}

void				Request::setBody(std::string &body)
{
	this->_body = body;
}



/*UTILS THAT WILL GO ELSEWHERE ONCE THIS FILE IS CLEANED UP*/

/*
	RETURN VALUE : Return true if the char 'c' is a space or a tab. Return
	false otherwise.
*/

static bool		isSpace(unsigned char c)
{
	return (c == ' ' || c == '\t');
}

/*
	Create and return a substring where leading and tailing spaces and tabs
	have been deleted from the string taken as argument.
	RETURN VALUE : The substring without leading and tailing whitespaces.
	If the string was empty or only composed of  whitespaces the function
	return an empty string.
*/

static std::string	trimSpaces(std::string str)
{
	std::string sub;
	size_t sub_len = str.length();
	size_t start, end;
	for (start = 0; start < sub_len && isSpace(str[start]); start++);
	for (end = sub_len-1; end >= 0 && isSpace(str[end]); end--);
	sub = str.substr(start, 1+end-start);
	return sub;
}

/*
	Take a string 'str' as parameter and put one line of it into the substr
	'line'. It uses a static size_t to determine the position in the string
	at each call of the function.
	RETURN VALUE : 0 if the last line has been reached. 1 if there is at
	least one other line after the one put in 'line'.
*/

static int		getNextLine(std::string const &str, std::string &line)
{
	static size_t pos = 0;
	static std::string const tmp;
	size_t tmp_pos = str.find("\r\n", pos);
	if (tmp_pos == std::string::npos)
	{
		line = str.substr(pos);
		pos = tmp_pos;
		return 0;
	}
	line = str.substr(pos, tmp_pos-(pos));
	pos = tmp_pos+2;
	return 1;
}

/*
	Take a line as parameter and check if there is a colon ':' to confirm
	that there is a separation 'field-name'/'field-value'. Will put the substr
	of the string 'line' before the colon in the string 'name'.
	RETURN VALUE : -1 if there is no colon. 400 if there is a space in the
	'field-name' part. 0 if the separation went correctly.
*/

static int		getFieldName(std::string const &line, std::string &name)
{
	size_t colon = line.find(':');
	if (colon == std::string::npos)
		return -1;
	name = line.substr(0, colon+1);
	if (name.find(' ') != std::string::npos)
		return 400;
	return 0;
}



/*NON-STATIC METHODS*/

int				Request::parseRequest(std::string const &request)
{
	std::string line;
	getNextLine(request, line);
	parseRequestLine(line);
	while (getNextLine(request, line))
		parseHeaderField(line);
	return (parseHeaderField(line));
}

int				Request::parseRequestLine(std::string const &request)
{
	//`start-line` ::= `method` SP `request-target` SP `HTTP-version` CRLF
	// std::string start(Header::_parseMethod(request))
	// return 0;
}

int				Request::parseHeaderField(std::string const &line)
{
	//`header-field` = `field-name` ":" OWS `field-value` OWS
	// field-value = *( field-content / obs-fold )
	// field-content = field-vchar [ 1*( SP / HTAB ) field-vchar ]
	// field-vchar = VCHAR / obs-text
	// obs-fold = CRLF 1*( SP / HTAB )
	// VCHAR = any visible USASCII char
	// obs-text = %x80-FF (UTF-8 chars written in hexa code, 80-FF refers to the char range outside ASCII)

	size_t		pos = 0;
	for (int j = 0; Request::_fieldNames[j][0]; j++)
	{
		std::string name;
		int getName = getFieldName(line, name);
		if (getName == -1)
			continue;
		else if (getName)
			return getName;

		std::string const str = Request::_fieldNames[j] + ':';
		if (!str.compare(name))
		{
			std::string s = trimSpaces(line.substr(str.length()));
			if (this->_headerFields[Request::_fieldNames[j]][0])
				this->_headerFields[Request::_fieldNames[j]] += ',' + s;
			else
				this->_headerFields[Request::_fieldNames[j]] = s;
			return 0;
		}
	}
	return 0;
}