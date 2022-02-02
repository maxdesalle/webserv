/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 14:57:25 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/02 16:34:27 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"

/*
	Some useful rules used in the parsing.
*/

std::string const	Header::_alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
std::string const	Header::_digit = "0123456789";
std::string const	Header::_unreserved = Header::_alpha + Header::_digit + "-._~";
std::string const	Header::_sub_delims = "!$&'()*+,;=";
std::string const	Header::_hexdig = Header::_digit + "ABCDEF";
std::string const	Header::_custom_pchar = Header::_unreserved + Header::_sub_delims + ":@";

/*
	The only methods that will be interpreted from a HTTP request.
	Other methods will be considered as random string of chars.
*/

std::string const 	Header::_authorizedMethods[4] = {"GET", "POST", "DELETE", ""};



/*CONSTRUCTORS AND DESTRUCTORS*/

Header::Header(void) {}

Header::Header(std::string const *fieldNames)
{
	for (int i = 0; fieldNames[i][0]; i++)
		this->_headerFields.insert(std::pair<std::string const, std::string>(fieldNames[i], ""));
}

Header::Header(Header const &src) {this->_headerFields = src._headerFields;}

Header::~Header(void) {}



/*OPERATOR OVERLOADS*/

Header const &Header::operator=(Header const &right)
{
	this->_headerFields = right._headerFields;
	return *this;
}



/*GETTERS AND SETTERS*/

std::string const	Header::getField(std::string const fieldName)
{
	if (this->_headerFields.find(fieldName) == this->_headerFields.end())
		return ("");
	return this->_headerFields[fieldName];
}

void				Header::setField(std::string const fieldName, std::string fieldValue)
{
	if (this->_headerFields.find(fieldName) == this->_headerFields.end())
		return ;
	this->_headerFields[fieldName] = fieldValue;
}



/*NON MEMBER FUNCTIONS*/

/*
	The "_parseX static methods" :
	X being the rule created in the RFCs.
	They all take the same parameters : _parseX(std::string &str, size_t pos)
	Their purpose is to check if the string 'str', starting at the position 'pos',
	contains the right sequence of chars as asked in the RFCs.
	RETURN VALUE : If the n first characters of the string correspond to the
	criterias of the function it will return this sequence as a substring of str.
	If not there is 2 solutions : some of the functions will return "" (most of
	the time it means that it is accepted by the RFCs to be an empty string but
	NOT ALWAYS) otherwise the functions will throw a WrongSyntaxException(). 
	WARNING : I DIDN'T TEST ALL OF THEM YET!!!
*/

/*
	Rule : HTTP-version = HTTP-name "/" DIGIT "." DIGIT
	Rule : HTTP-name = %x48.54.54.50 ; "HTTP"
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string			Header::_parseHTTPVersion(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (s.substr(0,5)!="HTTP/"
	|| Header::_digit.find(s[5])==std::string::npos
	|| s[6] != '.'
	|| Header::_digit.find(s[7])==std::string::npos)
		throw(Header::WrongSyntaxException());
	return s.substr(0, 8);
}

/*
	Rule : method = token
	The method tokens are defined in Header::_authorizedMethods.
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string			Header::_parseMethod(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	for (int i = 0; Header::_authorizedMethods[i]!=""; i++)
		if (Header::_authorizedMethods[i]==s.substr(0,Header::_authorizedMethods[i].length()))
			return Header::_authorizedMethods[i];
	throw(Header::WrongSyntaxException());
}

/*
	Rule : pct-encoded = "%" HEXDIG HEXDIG
	WARNING : Only return an empty string if syntax is not corresponding even
	if empty string is not accepted for this rule in the RFCs.
*/

std::string			Header::_parsePctEncoded(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (s[0] == '%'
	&& Header::_hexdig.find(s[1])!=std::string::npos
	&& Header::_hexdig.find(s[2])!=std::string::npos)
		return s.substr(0, 3);
	return "";
}

/*
	Rule : pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
	WARNING : Only return an empty string if syntax is not corresponding even
	if empty string is not accepted for this rule in the RFCs.
*/

std::string			Header::_parsePchar(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (Header::_custom_pchar.find(s[0]) != std::string::npos)
		return s.substr(0, 1);
	else
		return Header::_parsePctEncoded(s);
}

/*
	Rule : segment = *pchar
*/

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

/*
	Rule :  segment-nz = 1*pchar
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string			Header::_parseSegmentNz(std::string &str, size_t pos)
{
	std::string s = Header::_parseSegment(str, pos);
	if (s == "")
		throw(Header::WrongSyntaxException());
	return s;
}

/*
	Rule : query = *( pchar / "/" / "?" )
*/

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

/*
	Rule : path-abempty = *( "/" segment )
*/

std::string			Header::_parsePathAbempty(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string path = "";
	while (1)
	{
		size_t len = path.length();
		if (s[len] != '/')
			break ;
		path += '/' + Header::_parseSegment(s, len+1);
	}
	return path;
}

/*
	Rule : absolute-path = 1*( "/" segment )
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string			Header::_parseAbsPath(std::string &str, size_t pos)
{
	std::string s = Header::_parsePathAbempty(str, pos);
	if (s == "")
		throw(Header::WrongSyntaxException());
	return s;
}

/*
	Rule : path-rootless = segment-nz *( "/" segment )
	NOTE : path-rootless = segment-nz path-abempty
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string			Header::_parsePathRootless(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string path = Header::_parseSegmentNz(s);
	path += Header::_parsePathAbempty(s, path.length());
	return path;
}

/*
	Rule : path-absolute = "/" [ segment-nz *( "/" segment ) ]
	NOTE : path-absolute = "/" [ segment-nz path-abempty ]
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string			Header::_parsePathAbs(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (s[0] != '/')
		throw(Header::WrongSyntaxException());
	std::string path = "/";
	try
	{
		path += Header::_parseSegmentNz(s, 1);
		path += Header::_parsePathAbempty(s, path.length());
	}
	catch (const std::exception &e) {}
	return path;
}

/*
	Rule : scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string			Header::_parseScheme(std::string &str, size_t pos)
{
	std::string charArr = Header::_alpha + Header::_digit + "+-.";
	std::string s = str.substr(pos);
	std::string scheme = "";
	if (Header::_alpha.find(s[0]) == std::string::npos)
		throw(Header::WrongSyntaxException());
	scheme += s[0];
	for (int i = 1; i < s.length(); i++)
	{
		if (charArr.find(s[i]) != std::string::npos)
			scheme += s[i];
		else
			break ;
	}
	return scheme;
}

/*
	Rule : userinfo = *( unreserved / pct-encoded / sub-delims / ":" )
*/

std::string			Header::_parseUserInfo(std::string &str, size_t pos)
{
	std::string charArr = Header::_unreserved + Header::_sub_delims + ':';
	std::string s = str.substr(pos);
	std::string info = "";
	while (1)
	{
		size_t len = info.length();
		if (charArr.find(s[len]) != std::string::npos)
			info += s[len];
		else if (Header::_parsePctEncoded(s, len) != "")
			info += s.substr(len, 3);
		else
			break ;
	}
	return info;
}

/*
	Rule : reg-name = *( unreserved / pct-encoded / sub-delims )
*/

std::string			Header::_parseRegName(std::string &str, size_t pos)
{
	std::string charArr = Header::_unreserved + Header::_sub_delims;
	std::string s = str.substr(pos);
	std::string info = "";
	while (1)
	{
		size_t len = info.length();
		if (charArr.find(s[len]) != std::string::npos)
			info += s[len];
		else if (Header::_parsePctEncoded(s, len) != "")
			info += s.substr(len, 3);
		else
			break ;
	}
	return info;
}

/*
	Rule : port = *DIGIT
*/

std::string			Header::_parsePort(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string port = "";
	for (int i = 0; i < s.length(); i++)
	{
		if (Header::_digit.find(s[i]) != std::string::npos)
			port += s[i];
		else 
			break ;
	}
	return port;
}

/*
	Rule : dec-octet = DIGIT / %x31-39 DIGIT / "1" 2DIGIT / "2" %x30-34 DIGIT / "25" %x30-35
	NOTE : 0-9 or 10-99 or 100-199 or 200-249 or 250-255.
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string			Header::_parseDecOctet(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (s[0] < '0' || s[0] > '9')
		throw(Header::WrongSyntaxException());
	else if (s[1] < '0' || s[1] > '9')
		return s.substr(0, 1);
	else if (s[2] < '0' || s[2] > '9')
		return s.substr(0, 2);
	else if ((s[3] > '0' && s[3] < '9') || s[0] > '2'
	|| (s[0] == '2' && s[1] > '5')
	|| (s[0] == '2' && s[1] == '5' && s[2] > '5'))
		throw(Header::WrongSyntaxException());
	return s.substr(0, 3);
}

/*
	Rule : IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string			Header::_parseIpv4Address(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string ip = Header::_parseDecOctet(s);
	for (int i = 0; i < 3; i++)
	{
		size_t len = ip.length();
		if (s[len] != '.')
			throw(Header::WrongSyntaxException());
		ip += '.' + Header::_parseDecOctet(s, len);
	}
	return ip;
}

/*
	Rule : h16 = 1*4HEXDIG
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string			Header::_parseHexa16(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (Header::_hexdig.find(s[0]) == std::string::npos)
		throw(Header::WrongSyntaxException());
	std::string bytes(1, s[0]);
	for (int i = 1; i < 4; i++)
	{
		if (Header::_hexdig.find(s[0]) == std::string::npos)
			break ;
		bytes += s[i];
	}
	return bytes;
}

/*
	Rule : ls32 = ( h16 ":" h16 ) / IPv4address
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string			Header::_parseLow32(std::string&str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string bytes = "";
	try
	{
		bytes += Header::_parseIpv4Address(s);
	}
	catch(const std::exception& e)
	{
		bytes += Header::_parseHexa16(s);
		size_t len = bytes.length();
		if (s[len] != ':')
			throw(Header::WrongSyntaxException());
		bytes += ':' + Header::_parseHexa16(s ,len + 1);
	}
	return bytes;
}

/*
	Rule : IPv6address = 6( h16 ":" ) ls32
	 					/ "::" 5( h16 ":" ) ls32
	 					/ [ h16 ] "::" 4( h16 ":" ) ls32
	 					/ [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
	 					/ [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
	 					/ [ *3( h16 ":" ) h16 ] "::" h16 ":" ls32
	 					/ [ *4( h16 ":" ) h16 ] "::" ls32
						/ [ *5( h16 ":" ) h16 ] "::" h16
	 					/ [ *6( h16 ":" ) h16 ] "::"
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string			Header::_parseIpv6Address(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string ip = "";
	size_t position;
	for (position = 0; position < 6; position++)
	{
		size_t len = ip.length();
		try
		{
			ip += Header::_parseHexa16(s, len) + ':';
			if (s[ip.length()-1] != ':')
				throw(Header::WrongSyntaxException());
		}
		catch(const std::exception& e)
		{
			if (s[len] == ':')
			{
				ip += ':';
				break ;
			}
			throw(Header::WrongSyntaxException());
		}
	}
	if (position < 6)
	{
		for (position += 1; position < 6; position++)
		{
			ip += Header::_parseHexa16(s, ip.length());
			if (s[ip.length()] != ':')
				throw(Header::WrongSyntaxException());
		}
		ip += Header::_parseLow32(s, ip.length());
	}
	else
	{
		if (s[ip.length()] != ':')
		{
			try
			{
				ip += Header::_parseLow32(s, ip.length());
			}
			catch(const std::exception& e)
			{
				ip += Header::_parseHexa16(s, ip.length()) + ':';
				if (s[ip.length()-1] != ':')
					throw(Header::WrongSyntaxException());
			}
		}
		else
			ip += ':' + Header::_parseHexa16(s, ip.length() + 1); 
	}
	return ip;
}

/*
	Rule : IPvFuture = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string			Header::_parseIpvFuture(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (s[0] != 'v'
	|| Header::_hexdig.find(s[1]) == std::string::npos)
		throw(Header::WrongSyntaxException());
	std::string ip(s.substr(0, 2));
	for (int i = 2; i < s.length(); i++)
	{
		if (Header::_hexdig.find(s[i]) == std::string::npos)
			break ;
		ip += s[i];
	}
	std::string charArr = Header::_unreserved + Header::_sub_delims + ':';
	size_t len = ip.length();
	if (s[len] == '.'
	|| charArr.find(s[len+1]) == std::string::npos)
		throw(Header::WrongSyntaxException());
	ip += s[len] + s[len+1];
	for (int i = len + 2; i < s.length(); i++)
	{
		if (charArr.find(s[len+1]) == std::string::npos)
			break ;
		ip += s[i];
	}
	return ip;
}

/*
	Rule : IP-literal = "[" ( IPv6address / IPvFuture ) "]"
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string			Header::_parseIpLiteral(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (s[0] != '[')
		throw(Header::WrongSyntaxException());
	std::string ip = "[";
	try
	{
		ip += Header::_parseIpv6Address(s, 1);
	}
	catch(const std::exception& e)
	{
		ip += Header::_parseIpvFuture(s, 1);
	}
	if (s[ip.length()] != ']')
		throw(Header::WrongSyntaxException());
	return ip;
}

/*
	Rule : host = IP-literal / IPv4address / reg-name
	NOTE : As reg-name can be empty host can be empty too so no exception.
*/

std::string			Header::_parseHost(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string host = "";
	try
	{
		host += Header::_parseIpLiteral(s);
	}
	catch(const std::exception& e)
	{
		try
		{
			host += Header::_parseIpv4Address(s);
		}
		catch(const std::exception& e)
		{
			return Header::_parseRegName(s);
		}
		return host;
	}
	return host;
}

/*
	Rule : authority = [ userinfo "@" ] host [ ":" port ]
*/

std::string			Header::_parseAuthority(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string authority(Header::_parseUserInfo(s) + '@');
	if (s[authority.length()-1] != '@')
		authority = "";
	authority += Header::_parseHost(s, authority.length());
	if (s[authority.length()] == ':')
		authority += ':' + Header::_parsePort(s, authority.length()+1);
	return authority;
}

/*
	Rule : hier-part = "//" authority path-abempty / path-absolute / path-rootless / path-empty
	path-empty = 0<pchar>
	NOTE :	I'm not sure about the meaning of 0<rule>, [RFC3986] only say "zero characters"
			as a description of path-empty so for now path-empty == "".
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
	/!\ POTENTIAL MISTAKE : I'm not sure if the options are : 
		- "//" + authority + any kind of path
		- "//" + authority + path_abempty OR any other kind of path.
	I took the 1st solution but I have to check in the RFCs.
*/

std::string			Header::_parseHierPart(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (s.substr(0, 2) != "//")
		throw(Header::WrongSyntaxException());
	std::string part = "//" + Header::_parseAuthority(s, 2);
	std::string abempty = Header::_parsePathAbempty(s, part.length());
	if (abempty != "")
		return part + abempty;
	try
	{
		return part + Header::_parsePathAbs(s, part.length());
	}
	catch(const std::exception& e)
	{
		try
		{
			return part + Header::_parsePathRootless(s, part.length());
		}
		catch(const std::exception& e)
		{
			return part;
		}
	}
}

/*
	Rule : origin-form = absolute-path [ "?" query ]
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string		Header::_parseOriginForm(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string form(Header::_parseAbsPath(s));
	if (s[form.length()] == '?')
		form += '?' + Header::_parseQuery(s, form.length());
	return form;
}

/*
	Rule : absolute-form = absolute-URI
	Rule : absolute-URI = scheme ":" hier-part [ "?" query ]
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string		Header::_parseAbsForm(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string form(Header::_parseScheme(s) + ':');
	if (s[form.length() - 1] != ':')
		throw(Header::WrongSyntaxException());
	form += Header::_parseHierPart(s, form.length());
	if (s[form.length()] == '?')
		form += '?' + Header::_parseQuery(s, form.length());
	return form;
}

/*
	Rule : request-target = origin-form / absolute-form / authority-form / asterisk-form
	Rule : authority-form = authority
	Rule : asterisk-form = "*"
	NOTE : Authority-form basically might be an empty string so no exception.
*/

std::string		Header::_parseRequestTarget(std::string &str, size_t pos)
{
	std::string s = str.substr(pos);
	try
	{
		return Header::_parseOriginForm(s);
	}
	catch(const std::exception& e)
	{
		try
		{
			return Header::_parseAbsForm(s);
		}
		catch(const std::exception& e)
		{
			std::string target = Header::_parseAuthority(s);
			if (target != "" || (target == "" && s[0] != '*'))
				return target;
			return "*";
		}
	}
}


/*EXCEPTIONS MANAGEMENT*/

const char	*Header::WrongSyntaxException::what(void) const throw()
{
	return ("WrongSyntaxException, the content wasn't as intended for a request header.");
}