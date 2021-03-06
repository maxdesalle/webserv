/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucas <lucas@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 14:57:25 by ldelmas           #+#    #+#             */
/*   Updated: 2022/03/14 15:45:22 by lucas            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Header.hpp"

/*
	Some useful rules used in the parsing.
*/

std::string const	Header::alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
std::string const	Header::digit = "0123456789";
std::string const	Header::unreserved = Header::alpha + Header::digit + "-._~";
std::string const	Header::gen_delims = ":/?#[]@";
std::string const	Header::sub_delims = "!$&’()*+,;=";
std::string const	Header::reserved = Header::sub_delims + Header::gen_delims;
std::string const	Header::hexdig = Header::digit + "ABCDEF" + "abcdef";
std::string const	Header::custom_pchar = Header::unreserved + Header::sub_delims + ":@";

/*
	The only methods that will be interpreted from a HTTP request.
	Other methods will be considered as random string of chars.
*/

std::string const 	Header::authorizedMethods[5] = {"GET", "POST", "DELETE", "HEAD", ""};



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



/*MEMBER METHODS*/

bool				Header::findField(std::string const fieldName) const
{
	return (this->_headerFields.find(fieldName) != this->_headerFields.end());
}



/*NON MEMBER METHODS*/

/*
	RETURN VALUE : Return true if the char 'c' is a space or a tab. Return
	false otherwise.
*/

bool		Header::_isSpace(unsigned char c)
{
	return (c == ' ' || c == '\t');
}

/*
	RETURN VALUE : Return true if the char 'c' is a printable ASCII character.
	Return false otherwise.
*/

bool		Header::_isPrintable(unsigned char c)
{
	return (c > 31 && c < 128);	
}

/*
	Rule : obs-text = %x80-FF
	UTF-8 chars written in hexa code, 80-FF refers to the char range outside ASCII.
	RETURN VALUE : Return true if the char 'c' is a obs-text character.
	Return false otherwise.
*/

bool		Header::_isObsText(unsigned char c)
{
	return (c > 127);
}

/*
	Rule : field-vchar = VCHAR / obs-text
	RETURN VALUE : Return true if the char 'c' is a field-vchar character.
	Return false otherwise.
*/

bool		Header::_isFieldVchar(unsigned char c)
{
	return (c > 31 && c < 127);
}


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
*/

/*
	Rule : HTTP-version = HTTP-name "/" DIGIT "." DIGIT
	Rule : HTTP-name = %x48.54.54.50 ; "HTTP"
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string const	Header::_parseHTTPVersion(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (s.substr(0,5)!="HTTP/"
	|| Header::digit.find(s[5])==std::string::npos
	|| s[6] != '.'
	|| Header::digit.find(s[7])==std::string::npos)
		throw(Header::WrongSyntaxException());
	return s.substr(0, 8);
}

/*
	Rule : method = token
	The method tokens are defined in Header::_authorizedMethods.
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string const	Header::_parseMethod(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	for (int i = 0; Header::authorizedMethods[i]!=""; i++)
		if (Header::authorizedMethods[i]==s.substr(0,Header::authorizedMethods[i].length()))
			return Header::authorizedMethods[i];
	throw(Header::WrongSyntaxException());
}

/*
	Rule : pct-encoded = "%" HEXDIG HEXDIG
	WARNING : Only return an empty string if syntax is not corresponding even
	if empty string is not accepted for this rule in the RFCs.
*/

std::string const	Header::_parsePctEncoded(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (s[0] == '%'
	&& Header::hexdig.find(s[1])!=std::string::npos
	&& Header::hexdig.find(s[2])!=std::string::npos)
		return s.substr(0, 3);
	return "";
}

/*
	Rule : pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
	WARNING : Only return an empty string if syntax is not corresponding even
	if empty string is not accepted for this rule in the RFCs.
*/

std::string const	Header::_parsePchar(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (Header::custom_pchar.find(s[0]) != std::string::npos)
		return s.substr(0, 1);
	else
		return Header::_parsePctEncoded(s);
}

/*
	Rule : segment = *pchar
*/

std::string const	Header::_parseSegment(std::string const &str, size_t pos)
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

std::string const	Header::_parseSegmentNz(std::string const &str, size_t pos)
{
	std::string s = Header::_parseSegment(str, pos);
	if (s == "")
		throw(Header::WrongSyntaxException());
	return s;
}

/*
	Rule : query = *( pchar / "/" / "?" )
	Rule : fragment = query
*/

std::string const	Header::_parseQuery(std::string const &str, size_t pos)
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

std::string const	Header::_parsePathAbempty(std::string const &str, size_t pos)
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

std::string const	Header::_parseAbsPath(std::string const &str, size_t pos)
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

std::string const	Header::_parsePathRootless(std::string const &str, size_t pos)
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

std::string const	Header::_parsePathAbs(std::string const &str, size_t pos)
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
	catch (const std::exception &e) {return path;}
	return path;
}

/*
	Rule : scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string const	Header::_parseScheme(std::string const &str, size_t pos)
{
	std::string charArr = Header::alpha + Header::digit + "+-.";
	std::string s = str.substr(pos);
	std::string scheme = "";
	if (Header::alpha.find(s[0]) == std::string::npos)
		throw(Header::WrongSyntaxException());
	scheme += s[0];
	for (size_t i = 1; i < s.length(); i++)
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

std::string const	Header::_parseUserInfo(std::string const &str, size_t pos)
{
	std::string charArr = Header::unreserved + Header::sub_delims + ':';
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

std::string const	Header::_parseRegName(std::string const &str, size_t pos)
{
	std::string charArr = Header::unreserved + Header::sub_delims;
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

std::string const	Header::_parsePort(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string port = "";
	for (size_t i = 0; i < s.length(); i++)
	{
		if (Header::digit.find(s[i]) != std::string::npos)
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

std::string const	Header::_parseDecOctet(std::string const &str, size_t pos)
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

std::string const	Header::_parseIpv4Address(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string ip = Header::_parseDecOctet(s);
	for (size_t i = 0; i < 3; i++)
	{
		size_t len = ip.length();
		if (s[len] != '.')
			throw(Header::WrongSyntaxException());
		ip += '.' + Header::_parseDecOctet(s, len+1);
	}
	return ip;
}

/*
	Rule : h16 = 1*4HEXDIG
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string const	Header::_parseHexa16(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (Header::hexdig.find(s[0]) == std::string::npos)
		throw(Header::WrongSyntaxException());
	std::string bytes(1, s[0]);
	for (int i = 1; i < 4; i++)
	{
		if (Header::hexdig.find(s[i]) == std::string::npos)
			break ;
		bytes += s[i];
	}
	return bytes;
}

/*
	Rule : ls32 = ( h16 ":" h16 ) / IPv4address
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string const	Header::_parseLow32(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string bytes = "";
	try { bytes += Header::_parseIpv4Address(s); }
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

std::string const	Header::_parseIpv6Address(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string ip = "";
	bool empty = false;
	for (int i = 0; i < 8; i++)
	{
		if (i == 6 || empty) //all cases of l32 are managed here
		{
			try
			{
				ip += Header::_parseIpv4Address(s, ip.length());
				break ;
			}
			catch(const std::exception& e){}
		}
		if (s[ip.length()] == ':') //all cases of "::" are managed here
		{
			if (!empty)
			{
				if (ip == "")
					ip += "::";
				else
					ip += ':';
				empty = true;
				continue ;
			}
			else
				break ;
		}
		else //all cases of h16 and end of IP are managed here
		{
			if (!empty) // if no "::" before and isn't the 8th h16 must have a ':' after 
			{
				if (i == 7)
					ip += Header::_parseHexa16(s, ip.length());
				else
				{
					ip += Header::_parseHexa16(s, ip.length()) + ':';
					if (s[ip.length()-1] != ':')
						throw(Header::WrongSyntaxException());
				}
			}
			else //if a "::" occured any h16 can be the last so you have to check before adding the ':'
			{
				try
				{
					ip += _parseHexa16(s, ip.length());
					if (i == 7 || s[ip.length()] != ':')
						break ;
					else
					{
						if (i == 6)
						{
							try
							{
								Header::_parseIpv4Address(s, ip.length()+1);
								ip += ':';
							}
							catch(const std::exception& e){}
						}
						else
						{
							try
							{
								Header::_parseHexa16(s, ip.length()+1);
								ip += ':';
							}
							catch(const std::exception& e){}
						}
					}
				}
				catch(const std::exception& e){break;}
			}
		}
	}
	return ip;
}

/*
	Rule : IPvFuture = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string const	Header::_parseIpvFuture(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (s[0] != 'v'
	|| Header::hexdig.find(s[1]) == std::string::npos)
		throw(Header::WrongSyntaxException());
	std::string ip(s.substr(0, 2));
	for (size_t i = 2; i < s.length(); i++)
	{
		if (Header::hexdig.find(s[i]) == std::string::npos)
			break ;
		ip += s[i];
	}
	std::string charArr = Header::unreserved + Header::sub_delims + ':';
	size_t len = ip.length();
	if (s[len] != '.'
	|| charArr.find(s[len+1]) == std::string::npos)
		throw(Header::WrongSyntaxException());
	ip += s.substr(len, 2);
	for (size_t i = len + 2; i < s.length(); i++)
	{
		if (charArr.find(s[i]) == std::string::npos)
			break ;
		ip += s[i];
	}
	return ip;
}

/*
	Rule : IP-literal = "[" ( IPv6address / IPvFuture ) "]"
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string const	Header::_parseIpLiteral(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (s[0] != '[')
		throw(Header::WrongSyntaxException());
	std::string ip = "[";
	try { ip += Header::_parseIpv6Address(s, 1); }
	catch(const std::exception& e) { ip += Header::_parseIpvFuture(s, 1); }
	if (s[ip.length()] != ']')
		throw(Header::WrongSyntaxException());
	return ip + ']';
}

/*
	Rule : host = IP-literal / IPv4address / reg-name
	NOTE : As reg-name can be empty host can be empty too so no exception.
	NOTE : If the string is interpreted as an IPv4 it won't take the next
	chars even if a reg-name would have taken a longer substring. The same
	if interpreted as IP-literal.
*/

std::string const	Header::_parseHost(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	try { return Header::_parseIpLiteral(s); }
	catch(const std::exception& e)
	{
		try { return Header::_parseIpv4Address(s); }
		catch(const std::exception& e) { return Header::_parseRegName(s); }
	}
}

/*
	Rule : authority = [ userinfo "@" ] host [ ":" port ]
*/

std::string const	Header::_parseAuthority(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string authority(Header::_parseUserInfo(s) + '@');
	if ((authority != "@" && s[authority.length()-1] != '@') || authority == "@")
		authority = "";
	authority += Header::_parseHost(s, authority.length());
	if (s[authority.length()] == ':')
		authority += ':' + Header::_parsePort(s, authority.length()+1);
	return authority;
}

/*
	Rule : hier-part = "//" authority path-abempty
					/ path-absolute
					/ path-rootless
					/ path-empty
	path-empty = 0<pchar>
	NOTE :	As priority of operations has to be taken into account this rule means
			that the hier-part can be "//" authority path-abempty OR another type of
			path WITHOUT the concatenation of "//" authority before.
	NOTE :	I'm not sure about the meaning of 0<rule>, [RFC3986] only say "zero characters"
			as a description of path-empty so for now path-empty == "".
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string const	Header::_parseHierPart(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string part = "";
	if (s[0] == '/' && s[1] == '/')
	{
		part += "//" + Header::_parseAuthority(s, 2);
		part += Header::_parsePathAbempty(s, part.length());
		return part;
	}
	try { return Header::_parsePathAbs(s);}
	catch(const std::exception& e)
	{
		try { return Header::_parsePathRootless(s);}
		catch(const std::exception& e) {return "";}
	}
}

/*
	Rule : origin-form = absolute-path [ "?" query ]
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string const	Header::_parseOriginForm(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string form(Header::_parseAbsPath(s));
	if (s[form.length()] == '?')
		form += Header::_parseQuery(s, form.length());
	return form;
}

/*
	Rule : absolute-form = absolute-URI
	Rule : absolute-URI = scheme ":" hier-part [ "?" query ]
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string const	Header::_parseAbsForm(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string form(Header::_parseScheme(s) + ':');
	if (s[form.length() - 1] != ':')
		throw(Header::WrongSyntaxException());
	form += Header::_parseHierPart(s, form.length());
	if (s[form.length()] == '?')
		form += Header::_parseQuery(s, form.length());
	return form;
}

/*
	Rule : request-target = origin-form / absolute-form / authority-form / asterisk-form
	Rule : authority-form = authority
	Rule : asterisk-form = "*"
	NOTE : Authority-form basically might be an empty string so no exception.
*/

std::string const	Header::_parseRequestTarget(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	try { return Header::_parseOriginForm(s); }
	catch(const std::exception& e)
	{
		try { return Header::_parseAbsForm(s); }
		catch(const std::exception& e)
		{
			std::string target = Header::_parseAuthority(s);
			if (target != "" || (target == "" && s[0] != '*'))
				return target;
			return "*";
		}
	}
}

/*
	Do the same as above but change the 'type' string depending of the type of the
	target (origin-form, absolute-form, authority-form, asterisk-form).
*/

std::string const	Header::_parseRequestTarget(std::string &type, std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	try 
	{
		type = "ORIGIN";
		return Header::_parseOriginForm(s);
	}
	catch(const std::exception& e)
	{
		try
		{
			type = "ABSOLUTE";
			return Header::_parseAbsForm(s);
		}
		catch(const std::exception& e)
		{
			std::string target = Header::_parseAuthority(s);
			if (target != "" || (target == "" && s[0] != '*'))
			{
				type = "AUTHORITY";
				return target;
			}
			type = "ASTERISK";
			return "*";
		}
	}
}

/*
	Rule : URI = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
	WARNING : Will throw a WrongSyntaxException() if syntax is not corresponding.
*/

std::string const	Header::_parseURI(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	std::string form(Header::_parseScheme(s) + ':');
	if (s[form.length() - 1] != ':')
		throw(Header::WrongSyntaxException());
	form += Header::_parseHierPart(s, form.length());
	if (s[form.length()] == '?')
		form += Header::_parseQuery(s, form.length());
	if (s[form.length() == '#'])
		form += '#' + Header::_parseQuery(s, form.length() + 1);
	return form;
}

/*
	Rule : obs-fold = CRLF 1*( SP / HTAB )
	WARNING : Only return an empty string if syntax is not corresponding even
	if empty string is not accepted for this rule in the RFCs.
*/

std::string const	Header::_parseObsFold(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (s[0] != '\r' && s[1] != '\n' && s[2] != ' ' && s[2] != '	')
		return "";
	size_t i = 3;
	while (i < s.length() && (s[i] == ' ' || s[i] == '	'))
		i++;
	return s.substr(0, i);
}

/*
	Rule : field-content = field-vchar [ 1*( SP / HTAB ) field-vchar ]
	WARNING : Only return an empty string if syntax is not corresponding even
	if empty string is not accepted for this rule in the RFCs.
*/

std::string const	Header::_parseFieldContent(std::string const &str, size_t pos)
{
	std::string s = str.substr(pos);
	if (!Header::_isFieldVchar(static_cast<unsigned char>(s[0])))
		return "";
	size_t i = 1;
	while (i < s.length() && (s[i] == ' ' || s[i] == '	'))
		i++;
	if (i == 1 || !Header::_isFieldVchar(static_cast<unsigned char>(s[i])))
		return s.substr(0, 1);
	return s.substr(0, i+1);
}

/*
	Rule : field-value = *( field-content / obs-fold )
*/

std::string const	Header::_parseFieldValue(std::string const &str, size_t pos)
{
	std::string				s = str.substr(pos);
	std::string				value("");
	std::string				tmp = Header::_parseObsFold(s);
	if (tmp.empty())
		tmp = Header::_parseFieldContent(s);
	value += tmp;
	s = s.substr(1);
	while (!s.empty())
	{
		tmp = Header::_parseObsFold(s);
		tmp = Header::_parseObsFold(s);
		if (tmp.empty())
			tmp = Header::_parseFieldContent(s);
		value += tmp;
		s = s.substr(1);
	}
	return value;
}



/*EXCEPTIONS MANAGEMENT*/

const char	*Header::WrongSyntaxException::what(void) const throw()
{
	return ("WrongSyntaxException, the content wasn't as intended for a request header.");
}
