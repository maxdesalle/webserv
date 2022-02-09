/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 17:02:27 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/09 14:28:02 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "URI.hpp"

URI::URI(void) {}

URI::URI(URI const &src) {*this = src;}

URI::~URI(void) {}


URI	&URI::operator=(URI const &right)
{
	this->_HTTPForm = right._HTTPForm;
	this->_normalizedForm = right._normalizedForm;
	this->_scheme = right._scheme;
	this->_query = right._query;
	this->_fragment = right._fragment;
}

std::string const	&URI::getHTTPForm(void) const
{
	return this->_HTTPForm;
}

std::string const	&URI::getNormalizedForm(void) const
{
	return this->_normalizedForm;
}

void				URI::setHTTPForm(std::string const &str)
{
	this->_HTTPForm = str;
}

void				URI::setNormalizedForm(std::string const &str)
{
	this->_normalizedForm = str;
}

/*
	Normalize a string that is probably contained in a URI. 1st parameter is the
	string that will be normalized. 2nd is the position to start. 3rd is made for
	the user to put reserved characters that will still be allowed because of
	different reasons. 4th is used to know if hexadecimal-encoded value are
	authorized and must be converted into ASCII characters.
	RETURN VALUE : The normalized substring.
*/

std::string const	URI::_strNormalization(std::string &str, size_t pos,
										std::string const &except, bool pct)
{
	std::string s = str.substr(pos);
	size_t pos = 0;
	if (pct)
	{
		while ((pos = s.find('%', pos)) != std::string::npos)
		{
			int hval = Header::hexdig.find(s[pos+1]);
			int dval = Header::hexdig.find(s[pos+2]);
			if (pos >= s.length() - 2
			|| hval == std::string::npos
			|| dval == std::string::npos)
				throw(Header::WrongSyntaxException());
			unsigned char c = hval*16 + dval;
			s.replace(pos, 3, 1, c);
		}
	}
	for (size_t i = 0; i < s.length(); i++)
	{
		if (except.find(s[i] == std::string::npos)
		&& Header::reserved.find(s[i]) != std::string::npos)
			throw(Header::WrongSyntaxException());
		s[i] = std::towlower(s[i]);
	}
	return s;
}

/*
	Normalize the HTTP form URI which means that all hexadecimals are
	converted and uppercase letters become lowercase.
	Note that the URI is supposed to be correctly written and
	parsed at this point and another case would provoke a WrongSyntaxException().
	RETURN VALUE : 400 if wrong use of the reserved chars. 0 otherwise.
*/

int const			URI::_uriNormalization(void)
{
	this->_strNormalization(this->_scheme, 0, "+-.", false);
	if (this->_hpType == AUTHORITY)
	{
		
	}
	else if (this->_hpType == ABSOLUTE)
	{
		try
		{
			std::string segment = Header::_parseSegmentNz(this->_path, 1);
			this->_strNormalization(segment, 0, ":@"+Header::sub_delims, true);
			std::string abempty = Header::_parsePathAbempty(this->_path, segment.length());
			this->_strNormalization(abempty, 0, "/:@"+Header::sub_delims, true);
			this->_path = '/' + segment + abempty; 
		}
		catch(const std::exception& e) {}
	}
	else if (this->_hpType == ROOTLESS)
	{
		
	}
	this->_strNormalization(this->_query, 0, "/?:@"+Header::sub_delims, true);
	this->_strNormalization(this->_fragment, 0, "/?:@"+Header::sub_delims, true);
}

/*
	Rule : query = *( pchar / "/" / "?" )
	Rule : scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
	Rule : authority = [ userinfo "@" ] host [ ":" port ]
	Rule : userinfo = *( unreserved / pct-encoded / sub-delims / ":" )
	Rule : host = IP-literal / IPv4address / reg-name
	Rule : path-abempty = *( "/" segment )
	Rule : segment = *pchar
	Rule : pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
	Rule : path-absolute = "/" [ segment-nz path-abempty ]
	Rule : path-rootless = segment-nz path-abempty
	Rule : segment-nz = 1*pchar
*/

/*
	Rule : hier-part = "//" authority path-abempty
 						/ path-absolute
 						/ path-rootless
 						/ path-empty
	Use the hier-part attribute of the class to parse it into substrings for
	an easier normalization later. Also change the status of the type of
	hier-part since it will change which reserved characters are allowed and when.
*/

void				URI::_hierPartFormating(void)
{
	if (this->_hierPart.substr(0, 2) == "//")
	{
		this->_hpType = AUTHORITY;
		this->_auth = Header::_parseAuthority(this->_hierPart, 2);
		this->_path = Header::_parsePathAbempty(this->_hierPart, 2+this->_auth.length());
	}
	else
	{
		try
		{
			this->_path = Header::_parsePathAbs(this->_hierPart);
			this->_hpType = ABSOLUTE;
		}
		catch(const std::exception& e)
		{
			try
			{
				this->_path = Header::_parsePathRootless(this->_hierPart);
				this->_hpType = ROOTLESS;
			}
			catch(const std::exception& e) { this->_hpType = EMPTY;}
		}
	}
}

/*
	Rule : URI = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
	Use the HTTP form of the URI to stock in URI attributes the substring
	composing it.
*/

void				URI::_uriFormating(void)
{
	size_t total = 0;
	this->_scheme = Header::_parseScheme(this->_HTTPForm);
	total += this->_scheme.length()+1;
	this->_hierPart = Header::_parseHierPart(this->_HTTPForm, total);
	total += this->_hierPart.length();
	if (this->_HTTPForm[total] == '?')
	{
		this->_query = Header::_parseQuery(this->_HTTPForm, total+1);
		total += this->_query.length()+1;
	}
	if (this->_HTTPForm[total] == '#')
	{
		this->_fragment = Header::_parseQuery(this->_HTTPForm, total+1);
		total += this->_fragment.length()+1;
	}
}

