/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 14:59:17 by ldelmas           #+#    #+#             */
/*   Updated: 2022/01/28 14:10:04 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Request.hpp"

std::string const Request::_fieldNames[22] = {"Cache-Control", "Expect", "Host", 
										"Max-Forwards", "Pragma", "Range",
										"TE", "If-Match", "If-None-Match",
										"If-Modified-Since", "If-Unmodified-Since",
										"If-Range", "Accept", "Accept-Charset",
										"Accept-Encoding", "Accept-Language",
										"Authorization", "Proxy-Authorization",
										"From", "Referer", "User-Agent", ""};

Request::Request(void) : Header(Request::_fieldNames) {}

Request::Request(Request const &src) {this->headerFields = src.headerFields;}

Request::~Request(void) {}

Request const	&Request::operator=(Request const &right)
{
	this->headerFields = right.headerFields;
	return *this;
}

/*
	RETURN VALUE : Return true if the char 'c' is a space or a tab. Return
	false otherwise.
*/

static bool		isSpace(unsigned char c)
{
	return (c == ' ' || c == '\t');
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
	size_t tmp_pos = str.find('\n', pos);
	if (tmp_pos == std::string::npos)
	{
		line = str.substr(pos);
		pos = tmp_pos;
		return 0;
	}
	line = str.substr(pos, tmp_pos-(pos));
	pos = tmp_pos+1;
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

int				Request::parseRequest(std::string const &request)
{
	std::string line;
	while (getNextLine(request, line))
		parseRequestLine(line);
	return (parseRequestLine(line));
}

int				Request::parseRequestLine(std::string const &line)
{
	//`start-line` ::= `method` SP `request-target` SP `HTTP-version` CRLF
	//`header-field` = `field-name` ":" OWS `field-value` OWS

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
			std::string s = line.substr(str.length());
			s.erase(std::remove_if(s.begin(), s.end(), isSpace), s.end());
			if (this->headerFields[Request::_fieldNames[j]][0])
				this->headerFields[Request::_fieldNames[j]] += ',' + s;
			else
				this->headerFields[Request::_fieldNames[j]] = s;
			return 0;
		}
	}
	return 0;
}