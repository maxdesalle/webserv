/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 17:02:27 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/23 18:08:52 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "URI.hpp"

/*
	CONSTRUCTORS AND DESTRUCTORS
*/

/*
	Create the class with all string attributes as empty.
*/

URI::URI(void) : _HTTPForm(""), _normalizedForm(""), _scheme(""), _query(""),
				_fragment(""), _auth(""), _path(""), _hpType(UNDEFINED),
				_status(UNCHECKED) {}

/*
	Initialize the http form as the argument of the constructor.
	Do the formating and the normalization directly from there.
*/
URI::URI(std::string const &httpURI) : _HTTPForm(httpURI), _normalizedForm(""),
									_scheme(""), _query(""), _fragment(""),
									_auth(""), _path(""), _hpType(UNDEFINED),
									_status(UNCHECKED)
{
	this->_uriFormating();
	if (this->_uriNormalization())
		this->_status = WRONG_SYNTAX;
	else
		this->_status = NORMALIZED;	
}

URI::URI(URI const &src) {*this = src;}

URI::~URI(void) {}


/*
	OPERATION OVERLOADS
*/

/*
	Just copy all the attributes.
*/

URI	&URI::operator=(URI const &right)
{
	this->_HTTPForm = right._HTTPForm;
	this->_normalizedForm = right._normalizedForm;
	this->_scheme = right._scheme;
	this->_query = right._query;
	this->_fragment = right._fragment;
	this->_auth = right._auth;
	this->_path = right._path;
	this->_hpType = right._hpType;
	this->_status = right._status;
	return *this;
}


/*
	GETTERS AND SETTERS
*/

std::string const	&URI::getHTTPForm(void) const
{
	return this->_HTTPForm;
}

std::string const	&URI::getNormalizedForm(void) const
{
	return this->_normalizedForm;
}

std::string const	URI::getStatus(void) const
{
	switch (this->_status)
	{
		case NORMALIZED : return "NORMALIZED";
		case WRONG_SYNTAX : return "WRONG SYNTAX";
		case UNCHECKED : return "UNCHECKED";
	}
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
	UTILS PRIVATE METHODS
*/

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
	size_t p = 0;
	if (pct)
	{
		while ((p = s.find('%', p)) != std::string::npos)
		{
			size_t hval = Header::hexdig.find(s[p+1]);
			size_t dval = Header::hexdig.find(s[p+2]);
			if (p >= s.length() - 2
			|| hval == std::string::npos
			|| dval == std::string::npos)
				throw(Header::WrongSyntaxException());
			unsigned char c = hval*16 + dval;
			s.replace(p, 3, 1, c);
		}
	}
	for (size_t i = 0; i < s.length(); i++)
	{
		if (except.find(s[i]) == std::string::npos
		&& Header::reserved.find(s[i]) != std::string::npos)
			throw(Header::WrongSyntaxException());
		s[i] = std::towlower(s[i]);
	}
	return s;
}

/*
	Normalize the authority with its eventual components. The normalization
	highly depends on the presence of userinfo and port. It also depends on
	the type of value contained in host.
	RETURN VALUE : 0 if the normqlization didn't find anything wrong. 400
	otherwise.
*/

int				URI::_authNormalization(void)
{
	std::string userinfo = Header::_parseUserInfo(this->_auth);
	if (this->_auth[userinfo.length()] != '@')
		userinfo = "";
	std::string host = Header::_parseHost(this->_auth, userinfo.length()+1);
	size_t total = userinfo.length() + host.length() + 1;
	std::string port = "";
	if (this->_auth[total] == ':')
		port = ':' + Header::_parsePort(this->_auth, total+1);
	userinfo = this->_strNormalization(userinfo, 0, ':'+Header::sub_delims, true);
	if (host.substr(0, 2) == "[v")
	{
		size_t pos = host.find('.');
		std::string prefix = host.substr(0, pos);
		std::string suffix = host.substr(pos, host.length()-pos-1);
		suffix = this->_strNormalization(suffix, 0, ":" + Header::sub_delims,false);
		host = prefix + suffix + ']';
	}
	else if (host[0] != '[')
	{
		try {Header::_parseIpv4Address(host);}
		catch(const std::exception& e){host = this->_strNormalization(host, 0, Header::sub_delims, true);}
	}
	this->_auth = userinfo + '@' + host + port;
	return 0;
}

/*
	Normalize the HTTP form URI which means that all hexadecimals are
	converted and uppercase letters become lowercase.
	Note that the URI is supposed to be correctly written and
	parsed at this point and another case would provoke a WrongSyntaxException().
	RETURN VALUE : 400 if wrong use of the reserved chars. 0 otherwise.
*/

int				URI::_uriNormalization(void)
{
	this->_scheme = this->_strNormalization(this->_scheme, 0, "+-.", false);
	if (this->_hpType == AUTHORITY)
	{
		if (this->_authNormalization())
			return 400;
		this->_path = this->_strNormalization(this->_path, 0, "/:@"+Header::sub_delims, true);
		this->_hierPart = "//" + this->_auth + this->_path;
	}
	else if (this->_hpType == ABSOLUTE || this->_hpType == ROOTLESS)
	{
		std::string segment = Header::_parseSegment(this->_hierPart, 1);
		if (segment != "")
		{
			std::string abempty = Header::_parsePathAbempty(this->_hierPart, segment.length());
			segment = this->_strNormalization(segment, 0, ":@"+Header::sub_delims, true);
			abempty = this->_strNormalization(abempty, 0, "/:@"+Header::sub_delims, true);
			if (this->_hpType == ABSOLUTE)
				this->_hierPart = '/' + segment + abempty;
			else
				this->_hierPart = segment + abempty;
		}
	}
	this->_query = this->_strNormalization(this->_query, 0, "/?:@"+Header::sub_delims, true);
	this->_fragment = this->_strNormalization(this->_fragment, 0, "/?:@"+Header::sub_delims, true);
	this->_normalizedForm = this->_scheme + ':' + this->_hierPart;
	if (this->_query != "")
		this->_normalizedForm += '?' + this->_query;
	if (this->_fragment != "")
		this->_normalizedForm += '#' + this->_fragment;
	return 0;
}

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
			Header::_parsePathAbs(this->_hierPart);
			this->_hpType = ABSOLUTE;
		}
		catch(const std::exception& e)
		{
			try
			{
				Header::_parsePathRootless(this->_hierPart);
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
	std::string test = "";
	this->_scheme = Header::_parseScheme(this->_HTTPForm);
	total += this->_scheme.length()+1;
	this->_hierPart = Header::_parseHierPart(this->_HTTPForm, total);
	this->_hierPartFormating();
	total += this->_hierPart.length();
	test += this->_scheme + ':' + this->_hierPart;
	if (this->_HTTPForm[total] == '?')
	{
		this->_query = Header::_parseQuery(this->_HTTPForm, total+1);
		total += this->_query.length()+1;
		test += '?' + this->_query;
	}
	if (this->_HTTPForm[total] == '#')
	{
		this->_fragment = Header::_parseQuery(this->_HTTPForm, total+1);
		total += this->_fragment.length()+1;
		test += '#' + this->_fragment;
	}
	std::cout << test << std::endl;
	if (this->_HTTPForm != test)
		throw(Header::WrongSyntaxException());
}

