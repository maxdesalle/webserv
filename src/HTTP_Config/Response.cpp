/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 14:58:24 by ldelmas           #+#    #+#             */
/*   Updated: 2022/03/07 13:34:43 by mdesalle         ###   ########.fr       */
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
	*StatusCode = 500;
	return ("Internal Server Error");
}

std::string const		&Response::GetBadRequestResponse(Request &HTTPRequest, Location &HTTPLocation, unsigned int StatusCode)
{
	std::string			Body;
	std::ostringstream	oss;

	oss << std::dec << StatusCode;
	if (StatusCode != 301)
		Body = ReturnError(HTTPRequest, HTTPLocation, &StatusCode);
	_HeaderResponse = "HTTP/1.1 " + oss.str() + " " + FindStatusMessage(&StatusCode)+ "\r\n";
	if (StatusCode == 301)
		_HeaderResponse += "Location: " + HTTPRequest.getTarget() + "/" + "\r\n";
	_HeaderResponse += "Date: " + GetCurrentFormattedTime() + "\r\n";
	_HeaderResponse += "Server: " + GetServerVersion() + "\r\n";
	_HeaderResponse += Body;
	oss.str("");
	oss.clear();
	oss << std::dec << Body.size();
	_HeaderResponse += "Content-Length: " + oss.str() + "\r\n\r\n";

	std::cout << _HeaderResponse << std::endl;
	return (_HeaderResponse);
}

std::string const		&Response::GetHeaderResponse(Request &HTTPRequest, Location &HTTPLocation)
{
	std::string			Body;
	unsigned int		StatusCode = 0;
	std::ostringstream	oss;

	if (HTTPRequest.getMethod() == "GET")
		Body = HandleGETRequest(HTTPRequest, HTTPLocation, &StatusCode, 0);
	else if (HTTPRequest.getMethod() == "POST")
		Body = HandlePOSTRequest(HTTPRequest, HTTPLocation, &StatusCode);
	else if (HTTPRequest.getMethod() == "DELETE")
		Body = HandleDELETERequest(HTTPRequest, HTTPLocation, &StatusCode);
	else
	{
		StatusCode = 405; // Error 405: Method Not Allowed
		Body = ReturnError(HTTPRequest, HTTPLocation, &StatusCode);
	}

	oss << std::dec << StatusCode;
	_HeaderResponse =  "HTTP/1.1 " + oss.str() + " " + FindStatusMessage(&StatusCode) + "\r\n";
	_HeaderResponse += "Date: " + GetCurrentFormattedTime() + "\r\n";
	_HeaderResponse += "Server: " + GetServerVersion() + "\r\n";
	/* if (HTTPRequest.getMethod() == "GET") */
	/* 	_HeaderResponse += "Last-Modified: " + GetLastModifiedTimeForFile(Path) + "\n"; */
	oss.str("");
	oss.clear();
	oss << std::dec << Body.size();
	_HeaderResponse += "Content-Length: " + oss.str() + "\r\n\r\n";
	_HeaderResponse += Body;

	std::cout << _HeaderResponse << std::endl;

	return (_HeaderResponse);
}

std::string	Response::GetErrorPagePath(Location &HTTPLocation, unsigned int *StatusCode)
{
	std::string	Path;

	try
	{
		return (HTTPLocation.GetRoot() + HTTPLocation.GetPath() + HTTPLocation.GetErrorPage().at(*StatusCode));
	}
	catch (...)
	{
		return ("");
	}
}

std::string	Response::ReturnError(Request &HTTPRequest, Location &HTTPLocation, unsigned int *StatusCode)
{
	std::string			FileContent;
	std::string			Path = GetErrorPagePath(HTTPLocation, StatusCode);
	std::ifstream		File(Path.c_str());
	std::stringstream	Buffer;

	if (!File)
	{
		if (*StatusCode == 500)
			return ("500 Internal Server Error");
		*StatusCode = 500;
		return (ReturnError(HTTPRequest, HTTPLocation, StatusCode));
	}
	Buffer << File.rdbuf();
	FileContent = Buffer.str();
	return (FileContent);
}

std::string	Response::HandleGETRequest(Request &HTTPRequest, Location &HTTPLocation, unsigned int *StatusCode, unsigned int i)
{
	std::string			FileContent;
	std::string			Path = HTTPLocation.GetRoot() + HTTPRequest.getTarget() + HTTPLocation.GetIndex()[i];
	std::ifstream		File(Path.c_str());
	std::stringstream	Buffer;

	if (!File)
	{
		if (HTTPLocation.GetIndex().size() > (i + 1))
			return (HandleGETRequest(HTTPRequest, HTTPLocation, StatusCode, i + 1));
		else
		{
			*StatusCode = 404;
			return (ReturnError(HTTPRequest, HTTPLocation, StatusCode));
		}
	}
	Buffer << File.rdbuf();
	FileContent = Buffer.str();
	*StatusCode = 200;
	return (FileContent);
}

std::string	Response::HandlePOSTRequest(Request &HTTPRequest, Location &HTTPLocation, unsigned int *StatusCode)
{
	if (FindValueInVector(HTTPLocation.GetLimitExcept(), "POST") == false)
	{
		*StatusCode = 403;
		return (ReturnError(HTTPRequest, HTTPLocation, StatusCode));
	}

	if (HTTPLocation.isCgi() == true)
		return (HandleCGIPOSTRequest(HTTPRequest, StatusCode));
	else
		return (HandleNormalPostRequest(HTTPRequest, HTTPLocation, StatusCode));
}

std::string	Response::HandleNormalPostRequest(Request &HTTPRequest, Location &HTTPLocation, unsigned int *StatusCode)
{
	std::string		Path = HTTPLocation.GetRoot() + HTTPRequest.getTarget();
	std::ofstream	File(Path.c_str());

	if (!File)
	{
		*StatusCode = 500;
		return (ReturnError(HTTPRequest, HTTPLocation, StatusCode));
	}
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

std::string	Response::HandleDELETERequest(Request &HTTPRequest, Location &HTTPLocation, unsigned int *StatusCode)
{
	std::string	Path = HTTPLocation.GetRoot() + HTTPRequest.getTarget();

	std::cout << std::endl << std::endl << Path << std::endl << std::endl;
	
	if (FindValueInVector(HTTPLocation.GetLimitExcept(), "DELETE") == false)
	{
		*StatusCode = 403;
		return (ReturnError(HTTPRequest, HTTPLocation, StatusCode));
	}

	if (remove(Path.c_str()) != 0)
	{
		*StatusCode = 404;
		return (ReturnError(HTTPRequest, HTTPLocation, StatusCode));
	}
	return ("File deleted.");
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

/* std::string	Response::GetLastModifiedTimeForFile(std::string Path) */
/* { */
/* 	struct		stat attr; */
/* 	struct		tm *tm = gmtime(stat(Path.c_str(), &attr)); */
/* 	std::string	FormattedString; */

/* 	FormattedString.resize(29); */
/* 	strftime(&FormattedString[0], FormattedString.size(), "%a, %d %b %Y %X GMT", tm); */

/* 	return (FormattedString); */
/* } */

void				Response::reset(void)
{
	_HeaderResponse.clear();
}
