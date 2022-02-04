/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 14:59:17 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/04 16:47:50 by ldelmas          ###   ########.fr       */
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

Request::Request(void) : Header(Request::_fieldNames), _state(STARTLINE), _remain(""), _cursor(0) {}

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

/*
	Take a string 'str' as parameter and put one line of it into the substr
	'line'. It uses a static size_t to determine the position in the string
	at each call of the function.
	RETURN VALUE : 0 if the last line has been reached. 1 if there is at
	least one other line after the one put in 'line'.
*/

int				Request::_getNextLine(std::string const &str, std::string &line)
{
	size_t tmp_pos = str.find("\r\n", this->_cursor);
	if (tmp_pos == std::string::npos)
	{
		line = str.substr(this->_cursor);
		return 0;
	}
	line = str.substr(this->_cursor, tmp_pos-(this->_cursor));
	this->_cursor = tmp_pos+2; //might go too far if CRLF doesn't have anything after
	return 1;
}

/*
	Rule : start-line ::= method SP request-target SP HTTP-version CRLF
	Take the first line of the request and try to get the method, target and version.
	RETURN VALUE : If successful return 0 else depends on what is written in the RFCs
	(not done yet).
	/!\ I didn't check the right return values yet ! They will then be used for
	the response status.
*/

int				Request::_parseRequestLine(std::string const &request)
{
	std::string method, target, version;
	try { method = Header::_parseMethod(request); }
	catch(const std::exception& e) { return 400;}
	size_t len =method.length();
	if (request[len] != ' ')
		return 400;
	try { target = Header::_parseRequestTarget(request, len + 1);}
	catch(const std::exception& e) { return 400; }
	len += target.length() + 1;
	if (request[len] != ' ')
		return 400;
	try { version = Header::_parseHTTPVersion(request, len + 1);}
	catch(const std::exception& e) { return 400;}
	len += version.length() + 1;
	if (request.substr(len) != "")
		return 400;
	this->_method = method;
	this->_target = target;
	this->_version = version;
	return 0;
}

/*
	Rule : header-field = field-name ":" OWS field-value OWS
	Rule : field-value = *( field-content / obs-fold )
	Rule : field-content = field-vchar [ 1*( SP / HTAB ) field-vchar ]
	Rule : field-vchar = VCHAR / obs-text
	Rule : obs-fold = CRLF 1*( SP / HTAB )
	Rule : VCHAR = any visible USASCII char
	Rule : obs-text = %x80-FF (UTF-8 chars written in hexa code, 80-FF refers to the char range outside ASCII)
	Take a line as argument and check the syntax. Then check name of the field
	and trim the spaces around the field-value. The point is to associate a field-value
	to an already existing field-name.
	RETURN VALUE : 0 if everything went ok. 400 if syntax was wrong.
*/

int				Request::_parseHeaderField(std::string const &line)
{

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
	return 400;
}

/*
	Take a string as argument. This stirng is considered to be a chunk of the
	HTTP request. It will parse any chunk of the request and check its syntax.
	If syntax is ok all informations will be attributed where they have to.
	RETURN VALUE : 0 if everything was ok. If not the value returned depends on
	the RFCs (400 for now but can change and will influence the status of HTTP
	reponse).
*/

int				Request::parseRequest(std::string const &request)
{
	std::string line;
	this->_cursor = 0;
	std::string full = this->_remain + request;
	if (this->_state == STARTLINE)
	{
		if (!this->_getNextLine(full, line))
		{
			this->_remain = line;
			return 1;
		}
		if (this->_parseRequestLine(line))
			return 400;
		this->_state = HEADERS;
	}
	if (this->_state == HEADERS)
	{
		int ret = 0;
		while ((ret = this->_getNextLine(full, line)) && line != "")
			if (this->_parseHeaderField(line))
				return 400;
		if (!ret)
		{
			this->_remain = line;
			return 1;
		}
		this->_state = BODY;
	}
	if (this->_state == BODY)
	{
		if (this->_getNextLine(full, line))
		{
			this->_body = line;
			this->_state = PROCESSING;
		}
		else
		{
			this->_remain = line;
			return 1;
		}
	}
	return 0;
}
