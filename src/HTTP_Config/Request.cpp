/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <lucas@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 14:59:17 by ldelmas           #+#    #+#             */
/*   Updated: 2022/03/14 16:25:50 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Request.hpp"

/* Important static attributes*/

std::string const Request::_fieldNames[32] = {"Cache-Control", "Expect", "Host", 
										"Max-Forwards", "Pragma", "Range",
										"TE", "If-Match", "If-None-Match",
										"If-Modified-Since", "If-Unmodified-Since",
										"If-Range", "Accept", "Accept-Charset",
										"Accept-Encoding", "Accept-Language",
										"Authorization", "Proxy-Authorization",
										"From", "Referer", "User-Agent",
										"Transfer-Encoding", "Content-Length", 
										"Trailer", "Connection", "Via",
										"Received", "Warning", "Keep-Alive",
										"Upgrade", "Content-Type", ""};

std::string const Request::_cgiSerVarNames[ENV_NUM] = {"SERVER_SOFTWARE", "SERVER_NAME",
												"GATEWAY_INTERFACE", "SERVER_PROTOCOL",
												"SERVER_PORT", "REQUEST_METHOD",
												"PATH_INFO", "PATH_TRANSLATED",
												"SCRIPT_NAME", "QUERY_STRING",
												"REMOTE_HOST", "REMOTE_ADDR",
												"AUTH_TYPE", "REMOTE_USER",
												"AUTH_USER", "REMOTE_IDENT",
												"CONTENT_TYPE", "CONTENT_LENGTH"};


/*CONSTRUCTORS AND DESTRUCTORS*/

Request::Request(void) : Header(Request::_fieldNames), _remain(""), _cursor(0), _state(STARTLINE), _type(NONE), _expect(false)
{
	for (int i  = 0; i < ENV_NUM; i++)
		this->_cgiSerVars.insert(std::pair<std::string const, std::string>(_cgiSerVarNames[i], ""));
}

Request::Request(std::string const &request) : Header(Request::_fieldNames), _remain(""), _cursor(0), _state(STARTLINE), _type(NONE), _expect(false)
{
	for (int i  = 0; i < ENV_NUM; i++)
		this->_cgiSerVars.insert(std::pair<std::string const, std::string>(_cgiSerVarNames[i], ""));
	this->parseRequest(request);
}

Request::Request(Request const &src) {this->_headerFields = src._headerFields;}

Request::~Request(void) {}

/*OPERATOR OVERLOADS*/

Request			&Request::operator=(Request const &right)
{
	this->_headerFields = right._headerFields;
	this->_method = right._method;
	this->_target = right._target;
	this->_version = right._version;
	this->_body = right._body;
	this->_remain = right._remain;
	this->_cursor = right._cursor;
	this->_state = right._state;
	this->_type = right._type;
	for (int i = 0; i < ENV_NUM; i++)
	{
		std::string const &key = Request::_cgiSerVarNames[i];
		std::map<std::string const, std::string>::const_iterator it = right._cgiSerVars.find(key); 
		this->_cgiSerVars[key] = it->second;
	}
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

Request::state	Request::getState(void) const
{
	return this->_state;
}

bool	Request::isDone(void) const
{
	return this->_state == DONE;
}

std::map<std::string const, std::string> &Request::getCGIServerVars(void)
{
	return this->_cgiSerVars;
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

/*
	Executable name : WEBSERV/1.0
	CGI version : CGI/1.1
	Server protocol : HTTP/1.1
	This function should be called once the request-fields have been read.
	Important note : We suppose that we have an origin-form as a request target
	and that there is a "Host" header field. Which is the case if we do no bonuses
	for WebServ.
	/!\ Should this function take into account security? 
		-Path translated : do the file open? do we try to go /..?
		-What happens if no QUERY or no PORT?
		-What happens if wrong PORT, wrong server name, wrong path?
*/

void				Request::setCGIServerVars(Location const &CGILocation, in_addr_t addr)
{
	if (this->_state != DONE)
		return ;
	this->_cgiSerVars["SERVER_SOFTWARE"] = "WEBSERV/1.0";
	this->_cgiSerVars["SERVER_NAME"] = Header::_parseHost(this->_headerFields["Host"]);
	this->_cgiSerVars["GATEWAY_INTERFACE"] = "CGI/1.1";
	this->_cgiSerVars["SERVER_PROTOCOL"] = "HTTP/1.1";
	if (this->_headerFields["Host"][this->_cgiSerVars["SERVER_NAME"].length()] == ':')
		this->_cgiSerVars["SERVER_PORT"] = Header::_parsePort(this->_headerFields["Host"], this->_cgiSerVars["SERVER_NAME"].length()+1);
	this->_cgiSerVars["REQUEST_METHOD"] = this->_method;
	this->_cgiSerVars["PATH_INFO"] = Header::_parseAbsPath(this->_target);
	this->_cgiSerVars["PATH_TRANSLATED"] = CGILocation.GetRoot() + this->_cgiSerVars["PATH_INFO"];
	if (this->_cgiSerVars["PATH_INFO"][this->_cgiSerVars["PATH_INFO"].length()] == '?')
		this->_cgiSerVars["QUERY_STRING"] = Header::_parseQuery(this->_target, this->_cgiSerVars["PATH_INFO"].length()+1);
	// in_addr_t addr = Client.getIP();
	char buff[16];
	inet_ntop(AF_INET, &addr, buff, INET_ADDRSTRLEN);
	this->_cgiSerVars["REMOTE_ADDR"] = buff;
	this->_cgiSerVars["CONTENT_TYPE"] = this->_headerFields["Content-Type"];
	this->_cgiSerVars["CONTENT_LENGTH"] = this->_headerFields["Content-Length"];
	this->_cgiSerVars["SCRIPT_NAME"] = CGILocation.GetPass();

	/*IF SERVER DOESN'T SUPPORT USER AUTHENTIFICATION*/
	this->_cgiSerVars["AUTH_TYPE"] = "";
	this->_cgiSerVars["REMOTE_USER"] = "";
	this->_cgiSerVars["REMOTE_IDENT"] = "";
	/*ELSE*/
	//->get the auth-scheme part
	//this->_cgiSerVars["AUTH_TYPE"] = get_this->_headerFields["Authorization"];
	//->get the credentials part
	//this->_cgiSerVars["REMOTE_USER"] = get_this->_headerFields["Authorization"];
	//I'm really not sure of that but it seems that the username is also put in authorization
	// this->_cgiSerVars["REMOTE_IDENT"] = get_this->_headerFields["Authorization"];

	this->_cgiSerVars["AUTH_USER"] = this->_cgiSerVars["REMOTE_USER"];
}



/*UTILS THAT WILL GO ELSEWHERE ONCE THIS FILE IS CLEANED UP*/

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
	for (start = 0; start < sub_len && Header::_isSpace(str[start]); start++);
	for (end = sub_len-1; end >= 0 && Header::_isSpace(str[end]); end--);
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
	Put every attributes at their status when they were created at the class
	construction.
*/

void			Request::reset(void)
{
	this->_state = STARTLINE;
	for (int i = 0; this->_fieldNames[i][0]; i++)
		this->_headerFields[this->_fieldNames[i]] = "";
	for (int i = 0; i < ENV_NUM; i++)
		this->_cgiSerVars[this->_cgiSerVarNames[i]] = "";
	this->_method = "";
	this->_target = "";
	this->_version = "";
	this->_body = "";
	this->_remain = "";
	this->_cursor = 0;
	this->_expect = false;
}

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
	line = str.substr(this->_cursor, tmp_pos - this->_cursor);
	this->_cursor = tmp_pos+2; //might go too far if CRLF doesn't have anything after
	return 1;
}

/*
	Do the same a Request::_getNextLine but will only consider a new line if
	CRLF is not followed by SP or HTAB.
	RETURN VALUE : 0 if the last line has been reached. 1 if there is at
	least one other line after the one put in 'line'.
*/

int				Request::_getNextField(std::string const &str, std::string &line)
{
	size_t pos = str.find("\r\n", this->_cursor);
	// if (pos == std::string::npos || !str[pos] || str[pos] == ' ' || str[pos] == '	')
	// {
	// 	line = str.substr(this->_cursor);
	// 	return 0;
	// }
	line = str.substr(this->_cursor, pos - this->_cursor);
	if (line.empty())
		return 0;
	this->_cursor = pos+2;
	return 1;
}

/*
	Rule : start-line ::= method SP request-target SP HTTP-version CRLF
	Take the first line of the request and try to get the method, target and version.
	RETURN VALUE : If successful return 0 else depends on what is written in the RFCs
	(not done yet).
	Important note : An absolute-form request-target would be considered as an
	error since it is designed only for proxies. In case of using this code to
	create a proxy absolute-form would be authorized but not the origin-form.
	/!\ I didn't check the right return values yet ! They will then be used for
	the response status.
*/

int				Request::_parseRequestLine(std::string const &request)
{
	std::string method, target, version;
	try { method = Header::_parseMethod(request); }
	catch(const std::exception& e) { return 400;}
	size_t len = method.length();
	if (request[len] != ' ')
		return 400;
	std::string targetType = "";
	try { target = Header::_parseRequestTarget(targetType, request, len + 1);}
	catch(const std::exception& e) { return 400; }
	len += target.length() + 1;
	if (request[len] != ' '
	|| (targetType == "ASTERISK" && method != "OPTIONS")
	|| (targetType == "AUTHORITY" && method != "CONNECT")
	|| targetType == "ABSOLUTE")
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
	Take a line as argument and check the syntax. Then check name of the field
	and trim the spaces around the field-value. The point is to associate a field-value
	to an already existing field-name.
	RETURN VALUE : 0 if everything went ok. 400 if syntax was wrong. -1 if unfinished.
*/

int				Request::_parseHeaderField(std::string const &line)
{
	std::string name;
	if (getFieldName(line, name))
		return 400;
	for (int j = 0; Request::_fieldNames[j][0]; j++)
	{
		std::string const str = Request::_fieldNames[j] + ':';
		if (!str.compare(name))
		{
			std::string s = trimSpaces(line.substr(str.length()));
			if (Header::_parseFieldValue(s) != s)
				return 400;
			bool checkVal = Value::checkFieldValue(name, s);
			if (!checkVal && name == "Content-Type")
				return 415;
			else if (!checkVal)
				return 400;
			else if (this->_headerFields[Request::_fieldNames[j]][0])
				this->_headerFields[Request::_fieldNames[j]] += ',' + s;
			else
				this->_headerFields[Request::_fieldNames[j]] = s;
			if (j == 1 && !s.empty() && ci_equal(s, "100-continue"))
				_expect = true;	
			return 0;
		}
	}
	return 0;
}

/*
	Take a string as argument. This string is considered to be a chunk of the
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
		while ((ret = this->_getNextField(full, line)))
		{
			if (this->_parseHeaderField(line))
				return 400;
		}
		this->_cursor += 2;
		if ((ret = this->_findBodyType()))
			return ret;
	}
	if (_expect)
	{
		_expect = false;
		return 100;
	}
	if (this->_type != NONE)
		return this->_getBody(full);
	return 0;
}

int			Request::_findBodyType(void)
{
	char			*ptr = NULL;
	std::string		value;

	value = getField("Transfer-Encoding");
	if (!value.empty())
	{
		if (!ci_equal(value, "chunked"))
			return 501; // (Not Implemented)
		this->_body_size = 0;
		this->_type = CHUNKED;
		this->_chunk = SIZE;
		this->_state = BODY;
		return 0;
	}
	value = getField("Content-Length");
	if (!value.empty())
	{
		errno = 0;
		this->_body_size = strtol(value.c_str(), &ptr, 10);
		if (this->_body_size < 0 || ptr != &value[value.size()] || errno)
			return 400;
		this->_type = LEN;
		this->_state = BODY;
		return 0;
	}
	this->_state = DONE;
	return 0;
}

int			Request::_getBody(std::string const &buff)
{
	int				ret = 0;
	char			*ptr = NULL;
	std::string		line;

	if (this->_type == LEN) // Content-Length
	{
		// this->_getNextLine(buff, line);
		// if (line.empty())
		// 	return 0;
		if (buff.size() < this->_body_size)
		{
			this->_remain = buff;
			return 0;
		}
		this->_body = buff.substr(0, this->_body_size);
		this->_state = DONE;
		return 0;
	}
	else if (this->_chunk < TE) // Chunked : payload
	{
		while ((ret = this->_getNextField(buff, line)) && (this->_chunk < TE))
		{
			if (this->_chunk == SIZE)
			{
				errno = 0;
				this->_body_size = strtol(line.c_str(), &ptr, 16);
				if (this->_body_size < 0 || ptr <= line.c_str() || errno)
					return 400;
				if (this->_body_size)
					this->_chunk = DATA;
				else
				{
					this->_chunk = TE;
					this->_body_size = this->_body.size();
				}
			}
			else if (this->_chunk == DATA)
			{
				if (line.size() < this->_body_size)
				{
					this->_remain = line;
					return 0;
				}
				this->_body.append(line);
				this->_chunk = SIZE;
			}
		}
		if (this->_chunk < TE)
		{
			this->_remain = line;
			return 0;
		}
	}
	// Chunked : Trailer
	while ((ret = this->_getNextField(buff, line)))
		; // Chunked Trailer Part is ignored
	if (ret)
	{
		this->_remain = line;
		return 0;
	}
	this->_state = DONE;
	return 0;
}

std::ostream&	operator<<(std::ostream& stream, Request const& req)
{
	static const std::string	body_type[] = {"", "Content-Length", "Chunked"};

	stream	<< "\e[36m  Method: \e[0m" << req._method  << "\n" \
			<< "\e[36m  Target: \e[0m" << req._target  << "\n" \
			<< "\e[36m Version: \e[0m" << req._version << "\n" \
			<< "\e[36m Headers \e[0m";
	for (std::map<std::string const, std::string>::const_iterator it = req._headerFields.begin(); it != req._headerFields.end(); ++it)
	{
		if (it->second.empty())
			continue ;
		stream	<< "\n" << "\t \e[36m - " << it->first << ": \e[0m" << it->second;
	}
	if (!req._body.empty())
	{
		stream	<< "\n\e[36m  Body \e[0m";
		if (req._type)
			stream	<< " (" << body_type[req._type] << ")";
		stream	<< "\n" << req._body ;
	}
	return stream;
}

/*
	Print important information about the request in this order:
		-method
		-request-target
		-HTTP version
		-current state
		-any header field that have been set
		-body
*/

void		Request::printRequest(void)
{
	std::cout << "Method : " << this->_method << std::endl;
	std::cout << "Target : " << this->_target << std::endl;
	std::cout << "Version : " << this->_version << std::endl;
	switch (this->_state)
	{
		case STARTLINE :
			std::cout << "State : startline" << std::endl;
			break;
		case HEADERS :
			std::cout << "State : headers" << std::endl;
			break;
		case BODY :
			std::cout << "State : body" << std::endl;
			break;
		case DONE :
			std::cout << "State : done" << std::endl;
	}
	std::cout << "Header fields :" << std::endl;
	for (size_t i = 0; i < this->_headerFields.size(); i++)
		if (this->_headerFields[this->_fieldNames[i]] != "")
			std::cout << "\t-" << this->_fieldNames[i] << " : " << this->_headerFields[this->_fieldNames[i]];
}
