/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 14:59:05 by mdesalle          #+#    #+#             */
/*   Updated: 2022/03/16 14:38:52 by tderwedu         ###   ########.fr       */
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

std::string	Response::FindStatusMessage(unsigned int *StatusCode)
{
	switch (*StatusCode)
	{
		case 100:
			return ("Continue");
		case 101:
			return ("Switching Protocols");
		case 103:
			return ("Early Hints");
		case 200:
			return ("OK");
		case 201:
			return ("Created");
		case 202:
			return ("Accepted");
		case 203:
			return ("Non-Authoritative Information");
		case 204:
			return ("No Content");
		case 205:
			return ("Reset Content");
		case 206:
			return ("Partial Content");
		case 300:
			return ("Multiple Choices");
		case 301:
			return ("Moved Permanently");
		case 302:
			return ("Found");
		case 303:
			return ("See Other");
		case 304:
			return ("Not Modified");
		case 307:
			return ("Temporary Redirect");
		case 308:
			return ("Permanent Redirect");
		case 400:
			return ("Bad Request");
		case 401:
			return ("Unauthorized");
		case 402:
			return ("Payment Required");
		case 403:
			return ("Forbidden");
		case 404:
			return ("Not Found");
		case 405:
			return ("Method Not Allowed");
		case 406:
			return ("Not Acceptable");
		case 407:
			return ("Proxy Authentication Required");
		case 408:
			return ("Request Timeout");
		case 409:
			return ("Conflict");
		case 410:
			return ("Gone");
		case 411:
			return ("Length Required");
		case 412:
			return ("Precondition Failed");
		case 413:
			return ("Payload Too Large");
		case 414:
			return ("URI Too Long");
		case 415:
			return ("Unsupported Media Type");
		case 416:
			return ("Range Not Satisfiable");
		case 417:
			return ("Expectation Failed");
		case 418:
			return ("I'm a teapot");
		case 422:
			return ("Unprocessable Entity");
		case 425:
			return ("Too Early");
		case 426:
			return ("Upgrade Required");
		case 428:
			return ("Precondition Required");
		case 429:
			return ("Too Many Requests");
		case 431:
			return ("Request Header Fields Too Large");
		case 451:
			return ("Unavailable For Legal Reasons");
		case 500:
			return ("Internal Server Error");
		case 501:
			return ("Not Implemented");
		case 502:
			return ("Bad Gateway");
		case 503:
			return ("Service Unavailable");
		case 504:
			return ("Gateway Timeout");
		case 505:
			return ("HTTP Version Not Supported");
		case 506:
			return ("Variant Also Negotiates");
		case 507:
			return ("Insufficiant Storage");
		case 508:
			return ("Loop Detected");
		case 510:
			return ("Not Extended");
		case 511:
			return ("Network Authentication Required");
		default:
			break ;
	}
	*StatusCode = 200;
	return ("OK");
}

void			Response::setTargetPath(Request &HTTPRequest, Location *HTTPLocation)
{
	size_t 					pos;
	
	_target = HTTPRequest.getTarget();
	if (!HTTPLocation)
	{
		_target_path = _target;
		return ;
	}
	std::string const&		dir = HTTPLocation->GetPath();

	pos = dir.size() - (dir[dir.size() - 1] == '/');
	_target_path = HTTPLocation->GetRoot();
	_target_path.append(_target, pos, std::string::npos);
	return ;
}

std::string	const	&Response::GetBadRequestResponse(Request &HTTPRequest, Location *HTTPLocation, unsigned int StatusCode)
{
	std::string			Body;
	std::ostringstream	oss;

	// if (HTTPRequest.getMethod() != "GET" && HTTPRequest.getMethod() != "POST" && HTTPRequest.getMethod() != "DELETE")
	if (!HTTPLocation->isMethodValid(HTTPRequest.getMethod()))
	{
		StatusCode = 405;
		Body = "Method Not Allowed";
	}
	if (!HTTPLocation && Body.empty())
		Body = ReturnError(HTTPLocation, &StatusCode);
	if (Body.empty())
	{
		if (HTTPRequest.getBody().size() > HTTPLocation->GetClientMaxBodySize() && HTTPLocation->GetClientMaxBodySize() != std::string::npos)
			StatusCode = 413;
		if (StatusCode == 301)
			return (CheckIfFileOrFolderConst(HTTPRequest, HTTPLocation));
		else
			Body = ReturnError(HTTPLocation, &StatusCode);
	}
	oss << std::dec << StatusCode;
	_HeaderResponse = "HTTP/1.1 " + oss.str() + " " + FindStatusMessage(&StatusCode)+ "\r\n";
	_HeaderResponse += "Date: " + GetCurrentFormattedTime() + "\r\n";
	_HeaderResponse += "Server: " + GetServerVersion() + "\r\n";

	oss.str("");
	oss.clear();
	oss << std::dec << Body.size();
	_HeaderResponse += "Content-Length: " + oss.str() + "\r\n\r\n";
	_HeaderResponse += Body;

	// std::cout << _HeaderResponse << std::endl;
	return (_HeaderResponse);
}

std::string const		&Response::GenerateResponse(std::string &Body, unsigned int *StatusCode)
{
	std::ostringstream	oss;

	oss << std::dec << *StatusCode;
	
	if (!_HeaderResponse.empty())
		return (_HeaderResponse);

	_HeaderResponse =  "HTTP/1.1 " + oss.str() + " " + FindStatusMessage(StatusCode) + "\r\n";
	_HeaderResponse += "Date: " + GetCurrentFormattedTime() + "\r\n";
	_HeaderResponse += "Server: " + GetServerVersion() + "\r\n";

	oss.str("");
	oss.clear();
	oss << std::dec << Body.size();

	_HeaderResponse += "Content-Length: " + oss.str() + "\r\n\r\n";
	_HeaderResponse += Body;

	return (_HeaderResponse);
}

bool					Response::RedirectionExists(Location *HTTPLocation)
{
	try
	{
		if (!HTTPLocation->GetReturn().at(301).empty())
			return (true);
	}
	catch (...)
	{
		return (false);
	}
	return (false);
}

std::string	const	&Response::HandleRedirection(Request &HTTPRequest, Location *HTTPLocation)
{
	size_t	Split;
	std::string		RootURL = HTTPLocation->GetReturn().at(301);

	if ((Split = RootURL.find('$')) != std::string::npos)
	{
		RootURL = RootURL.substr(0, Split);
		RootURL += HTTPRequest.getTarget();
	}

	_HeaderResponse = "HTTP/1.1 301 Moved Permanently\r\n";
	_HeaderResponse += "Date: " + GetCurrentFormattedTime() + "\r\n";
	_HeaderResponse += "Server: " + GetServerVersion() + "\r\n";
	_HeaderResponse += "Location: " + RootURL + "\r\n";
	_HeaderResponse += "Content-Length: 0\r\n\r\n";

	return (_HeaderResponse);
}


std::string const		&Response::GetHeaderResponse(Request &HTTPRequest, Location *HTTPLocation)
{
	std::string			Body;
	unsigned int		StatusCode = 200;

	setTargetPath(HTTPRequest, HTTPLocation);
	// if (HTTPRequest.getMethod() != "GET" && HTTPRequest.getMethod() != "POST" && HTTPRequest.getMethod() != "DELETE")
	if (!HTTPLocation->isMethodValid(HTTPRequest.getMethod()))
	{
		StatusCode = 405;
		Body = ReturnError(HTTPLocation, &StatusCode);
	}

	if (HTTPRequest.getBody().size() > HTTPLocation->GetClientMaxBodySize() && HTTPLocation->GetClientMaxBodySize() != std::string::npos)
	{
		StatusCode = 413;
		Body = ReturnError(HTTPLocation, &StatusCode);
	}

	if (RedirectionExists(HTTPLocation))
		return (HandleRedirection(HTTPRequest, HTTPLocation));

	if (Body.empty())
	{
		if (HTTPRequest.getMethod() == "GET")
			Body = CheckIfFileOrFolder(HTTPRequest, HTTPLocation, &StatusCode);
		else if (HTTPRequest.getMethod() == "POST")
			Body = HandlePOSTRequest(HTTPRequest, HTTPLocation, &StatusCode);
		else if (HTTPRequest.getMethod() == "DELETE")
			Body = HandleDELETERequest(HTTPLocation, &StatusCode);
	}

	GenerateResponse(Body, &StatusCode);

	// std::cout << _HeaderResponse << std::endl; //TODO:remove
	return (_HeaderResponse);
}

std::string	Response::GetErrorPagePath(Location *HTTPLocation, unsigned int *StatusCode)
{
	std::map<size_t, std::string> const&			error_pages = HTTPLocation->GetErrorPage();
	std::map<size_t, std::string>::const_iterator	it;

	if (HTTPLocation != NULL)
		it = error_pages.find(*StatusCode);
	else
		return ("");
	if (it != error_pages.end())
		return it->second;
	return ("");
}

std::string Response::ReturnError(Location *HTTPLocation, unsigned int *StatusCode)
{
	std::string			FileContent;
	std::stringstream	Buffer;
	std::string			Path = GetErrorPagePath(HTTPLocation, StatusCode);
	std::string			error;

	if (!Path.empty() && is_file(Path)) {
		std::ifstream		File(Path.c_str());
		Buffer << File.rdbuf();
		FileContent = Buffer.str();
	} else {
		Buffer << *StatusCode;
		error += Buffer.str();
		error += " ";
		error += FindStatusMessage(StatusCode);
		FileContent += "<html>\n\t<head>\n\t\t<title>\n\t\t\t";
		FileContent += error;
		FileContent += "\n\t\t</title>\n\t</head>\n\t<body>\n\t\t<center>\n\t\t\t<h1>\n<br>\n\t\t\t\t";
		FileContent += error;
		FileContent += "\n\t\t\t</h1>\n<br><br><hr>\n\t\t\t<footer>\n\t\t\t\tWebserv<br><small>";
		FileContent += "by ldelmas, mdesalle and tderwedu</small>\n\t\t\t</footer>\n\t\t</center>\n\t</body>\n</html>";
	}
	return (FileContent);
}

std::string const	&Response::Handle301Redirect(void)
{
	_HeaderResponse = "HTTP/1.1 301 Moved Permanently\r\n";
	_HeaderResponse += "Location: " + _target + "/" + "\r\n";
	_HeaderResponse += "Date: " + GetCurrentFormattedTime() + "\r\n";
	_HeaderResponse += "Server: " + GetServerVersion() + "\r\n";
	_HeaderResponse += "Content-Length: 0\r\n\r\n";
	_Body = "";

	return (_Body);
}

std::string Response::CheckIfFileOrFolder(Request &HTTPRequest, Location *HTTPLocation, unsigned int *StatusCode)
{
	struct				stat s;

	if (stat(_target_path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFREG)
			return (HandleGETRequestFile(HTTPRequest, HTTPLocation, StatusCode));
		else if (s.st_mode & S_IFDIR)
		{
			if (_target[_target.size() - 1] != '/')
				return (Handle301Redirect());
			return (HandleGETRequest(HTTPRequest, HTTPLocation, StatusCode, 0));
		}
	}
	return (HandleGETRequest(HTTPRequest, HTTPLocation, StatusCode, 0));
}

std::string const &Response::CheckIfFileOrFolderConst(Request &HTTPRequest, Location *HTTPLocation)
{
	struct				stat s;

	if (stat(_target_path.c_str(),&s) == 0)
	{
		if (s.st_mode & S_IFREG)
			return (GetHeaderResponse(HTTPRequest, HTTPLocation));
		else if (s.st_mode & S_IFDIR)
		{
			if (_target[_target.size()] != '/' && HTTPRequest.getMethod() == "GET")
				return (Handle301Redirect());
			return (GetHeaderResponse(HTTPRequest, HTTPLocation));
		}
	}
	return (GetHeaderResponse(HTTPRequest, HTTPLocation));
}

std::string	Response::HandleGETCGIRequest(Request &HTTPRequest, Location *HTTPLocation, unsigned int *StatusCode)
{
	if (HTTPLocation->GetPass().empty())
	{
		*StatusCode = 500;
		return (ReturnError(HTTPLocation, StatusCode));
	}

	CommonGatewayInterface	CGI(HTTPRequest);
	*StatusCode = CGI.ExecuteCGIScript();
	return (CGI.getBody());
}

std::string	Response::HandleGETRequestFile(Request &HTTPRequest, Location *HTTPLocation, unsigned int *StatusCode)
{
	std::string			FileContent;
	std::ifstream		File(_target_path.c_str());
	std::stringstream	Buffer;

	if (HTTPRequest.getField("Content-Type").substr(0, 11) == "application")
		return (HandleGETCGIRequest(HTTPRequest, HTTPLocation, StatusCode));

	if (FindValueInVector(HTTPLocation->GetLimitExcept(), "GET") == false)
	{
		*StatusCode = 403;
		return (ReturnError(HTTPLocation, StatusCode));
	}

	if (!File)
		return (HandleGETRequest(HTTPRequest, HTTPLocation, StatusCode, 0));

	Buffer << File.rdbuf();
	FileContent = Buffer.str();
	*StatusCode = 200;

	return (FileContent);
}

std::string	Response::GetPath(Location *HTTPLocation, unsigned int i)
{
	if (i < HTTPLocation->GetIndex().size())
		return (_target_path + HTTPLocation->GetIndex()[i]);
	else
		return ("");
}

std::string	Response::HandleGETRequest(Request &HTTPRequest, Location *HTTPLocation, unsigned int *StatusCode, unsigned int i)
{
	std::string			FileContent;
	std::string			Path = GetPath(HTTPLocation, i);
	std::ifstream		File(Path.c_str());
	std::stringstream	Buffer;


	if (FindValueInVector(HTTPLocation->GetLimitExcept(), "GET") == false)
	{
		*StatusCode = 405;
		return (ReturnError(HTTPLocation, StatusCode));
	}

	if (!File)
	{
		if (HTTPLocation->GetIndex().size() > (i + 1))
			return (HandleGETRequest(HTTPRequest, HTTPLocation, StatusCode, i + 1));
		else
		{
			if (HTTPLocation->GetAutoIndex() && is_dir(_target_path))
				return (get_autoindex(_target_path, _target));
			else
			{
				*StatusCode = 404;
				return (ReturnError(HTTPLocation, StatusCode));
			}
		}
	}
	Buffer << File.rdbuf();
	FileContent = Buffer.str();
	*StatusCode = 200;
	return (FileContent);
}

std::string	Response::HandlePOSTRequest(Request &HTTPRequest, Location *HTTPLocation, unsigned int *StatusCode)
{
	if (FindValueInVector(HTTPLocation->GetLimitExcept(), "POST") == false)
	{
		// std::cout << HTTPRequest.getTarget() << ": " << HTTPRequest.getTarget()[HTTPRequest.getTarget().size() + 1] << std::endl;
		if (HTTPRequest.getTarget()[HTTPRequest.getTarget().size() + 1] != '/')
			*StatusCode = 400;
		else
			*StatusCode = 405;
		return (ReturnError(HTTPLocation, StatusCode));
	}

	if (HTTPRequest.getField("Content-Type").substr(0, 11) == "application")
		return (HandleCGIPOSTRequest(HTTPRequest, StatusCode));
	else
		return (HandleNormalPostRequest(HTTPRequest, HTTPLocation, StatusCode));
}

std::string	Response::HandleNormalPostRequest(Request &HTTPRequest, Location *HTTPLocation, unsigned int *StatusCode)
{
	std::ofstream	File(_target_path.c_str());

	if (!File)
	{
		*StatusCode = 500;
		return (ReturnError(HTTPLocation, StatusCode));
	}
	*StatusCode = 200;
	File << HTTPRequest.getBody();
	return ("");
}

std::string	Response::HandleCGIPOSTRequest(Request &HTTPRequest, unsigned int *StatusCode)
{
	CommonGatewayInterface	CGI(HTTPRequest);

	*StatusCode = CGI.ExecuteCGIScript();
	return ("");
}

bool		Response::FindValueInVector(std::vector<std::string> Haystack, std::string Needle)
{
	for (size_t i = 0; i < Haystack.size(); i += 1)
	{
		if (Haystack[i] == Needle)
			return (true);
	}
	return (false);
}

std::string	Response::HandleDELETERequest(Location *HTTPLocation, unsigned int *StatusCode)
{
	std::string	FileContent;

	if (FindValueInVector(HTTPLocation->GetLimitExcept(), "DELETE") == false)
	{
		*StatusCode = 405;
		return (ReturnError(HTTPLocation, StatusCode));
	}

	if (remove(_target_path.c_str()) != 0)
	{
		*StatusCode = 404;
		return (ReturnError(HTTPLocation, StatusCode));
	}

	*StatusCode = 200;

	FileContent += "<html>\n\t<head>\n\t\t<title>\n\t\t\t";
	FileContent += "File deleted";
	FileContent += "\n\t\t</title>\n\t</head>\n\t<body>\n\t\t<center>\n\t\t\t<h1>\n<br>\n\t\t\t\t";
	FileContent += "File deleted: " + _target_path; //TODO: Should it be '_target' or '_target_path' ??
	FileContent += "\n\t\t\t</h1>\n<br><br><hr>\n\t\t\t<footer>\n\t\t\t\tWebserv<br><small>";
	FileContent += "by ldelmas, mdesalle and tderwedu</small>\n\t\t\t</footer>\n\t\t</center>\n\t</body>\n</html>";

	return (FileContent);
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

void				Response::reset(void)
{
	_HeaderResponse.clear();
}
