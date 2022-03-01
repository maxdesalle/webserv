/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Value.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 16:26:58 by ldelmas           #+#    #+#             */
/*   Updated: 2022/03/01 16:17:38 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Value.hpp"

const std::string Value::_tcoding[4] = {"chunked", "compress", "deflate", "gzip"};
const std::string Value::_types[NTYPE] = {"text", "image", "audio", "video", "application"};
const std::string Value::_sub_text[NTEXT] = {"plain"}; //anything unrecognized is text/plain if known charset
const std::string Value::_sub_image[NIMG] = {"jpeg", "png", "gif", "bmp", "webp"};
const std::string Value::_sub_audio[NAUDIO] = {"basic", "midi", "wav", "mpeg", "webm", "ogg"};
const std::string Value::_sub_video[NVIDEO] = {"webm", "ogg"};
const std::string Value::_sub_app[NAPP] = {"octet-stream", "postscript"};
const std::string *Value::_stypes[5] = {Value::_sub_text, Value::_sub_image, Value::_sub_audio,
										Value::_sub_video, Value::_sub_app};

std::map<std::string const, std::string const*>				Value::_subtypes;
std::map<std::string const, int const>						Value::_num_stypes;
std::map<std::string const, bool(&)(std::string &value)>	Value::_checkers;



/*
	CONSTRUCTORS AND DESTRUCTORS
*/

Value::Value(void) {}

Value::Value(Value const &src)
{
	*this = src;
}


Value::~Value(void) {};



/*
	OPERATION OVERLOADS
*/

Value	&Value::operator=(Value const &right)
{
	(void)right;
	return *this;
};



/*
	PUBLIC NON MEMBER METHODS
*/

/*
	Check if the key-value is syntaxically correct given the key-name that define
	the syntaxic rules.
	RETURN VALUE : If the key-name is not found there is no specific rule to check
	if the value is correctly written. In that case the method return true.
	If the key-name is found the corresonding static method in the class is called
	and return true is the key-value is correctly written and false otherwise.
*/

bool	Value::checkFieldValue(std::string const &name, std::string &value)
{
	Value::_initialize();
	if (Value::_checkers.find(name) == Value::_checkers.end())
		return true;
	return (Value::_checkers.find(name)->second)(value);
}


/*
	PRIVATE NON MEMBER METHODS
*/

/*
	Initialize every map needed to check everything. Is only called once on the program.
*/

void				Value::_initialize(void)
{
	if (Value::_subtypes.size() == 0)
	{
		for (int i = 0; i < NTYPE; i++)
			Value::_subtypes.insert(subPair(Value::_types[i], Value::_stypes[i]));
		Value::_num_stypes.insert(std::pair<std::string const, int const>("text", NTEXT));
		Value::_num_stypes.insert(std::pair<std::string const, int const>("image", NIMG));
		Value::_num_stypes.insert(std::pair<std::string const, int const>("audio", NAUDIO));
		Value::_num_stypes.insert(std::pair<std::string const, int const>("video", NVIDEO));
		Value::_num_stypes.insert(std::pair<std::string const, int const>("application", NAPP));

		Value::_checkers.insert(valPair("Host", Value::_checkHost));
		Value::_checkers.insert(valPair("Content-Length", Value::_checkContentLength));
		Value::_checkers.insert(valPair("Transfer-Encoding", Value::_checkTransferEncoding));
		Value::_checkers.insert(valPair("Content-Type", Value::_checkContentType));
		Value::_checkers.insert(valPair("Accept", Value::_checkAccept));
		Value::_checkers.insert(valPair("Expect", Value::_checkExpect));
	}
}

/*
	The 'Value::_get' methods :
	They take a portion of a string to check if corresponding to what is expected.
	RETURN VALUE : If the substring correspond to an expectation it will be returned.
	Else an empty stirng will be returned.
*/

std::string const	Value::_getType(std::string const &value)
{
	for (int i = 0; i < NTYPE; i++)
	{
		size_t tlen = Value::_types[i].length();
		if (!value.compare(0, tlen, Value::_types[i]))
			return Value::_types[i];
	}
	return "";
}

std::string const	Value::_getSubtype(std::string const &type, std::string const &value)
{
	for (int j = 0; j < Value::_num_stypes[type]; j++)
	{
		size_t len = Value::_subtypes[type][j].length();
		if (!value.compare(0,len, Value::_subtypes[type][j]))
			return Value::_subtypes[type][j] ;
	}
	return "";
}

std::string const Value::_getAccept(std::string const &value)
{
	std::string const &type = Value::_getType(value)==""?(value[0]== '*'?"*":""):Value::_getType(value);
	if (value[type.length()] != '/')
		return type;
	else if (type == "")
		return "";
	if (value[type.length()+1] == '*')
		return type + "/*";
	std::string const &subtype = Value::_getSubtype(type, value.substr(type.length()+1));
	if (subtype == "")
		return "";
	return type + '/' + subtype;
}

/*
	The 'Value::_check' methods :
	Each one of them correspond to the key-name of a header field and will be
	checking if the corresponding value is correct. They will return true or false
	depending on that. They are stocked in the Value::_checkers map permissing to
	be called directly from the header-field key-name.
	Example : Value::_checkers["Host"] actually calls Value::_checkHost.
*/

/*
	Case sensitive? Only IPs and reg-name -> not our problem.
*/

bool	Value::_checkHost(std::string &value)
{
	std::string uriHost = Header::_parseHost(value);
	if (uriHost == "" && uriHost != value)
		return false;
	if (value[uriHost.length()] == ':')
	{
		uriHost += ':' + Header::_parsePort(value, uriHost.length()+1);
		if (uriHost != value)
			return false ;
		return true;
	}
	else if (value[uriHost.length()])
		return false;
	return true;
}

/*
	Case sensitive? Only cyphers -> unrelevant.
*/

bool	Value::_checkContentLength(std::string &value)
{
	std::string length = Header::_parsePort(value);
	if (length == "" || length != value)
		return false;
	return true;
}

/*
	Rules :
		Transfer-Encoding = 1#transfer-coding
		transfer-coding = "chunked" / "compress" / "deflate" / "gzip" / transfer-extension
		transfer-extension = token *( OWS ";" OWS transfer-parameter )
		transfer-parameter = token BWS "=" BWS ( token / quoted-string )
		quoted-string = DQUOTE *( qdtext / quoted-pair ) DQUOTE
		qdtext = HTAB / SP /%x21 / %x23-5B / %x5D-7E / obs-text
		obs-text = %x80-FF
		quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )
	Case sensitive? Yes as there is no detail on it in RFCs but should compare to nginx.
	Check for the Transfer-Encoding header field. Note that there can be multiple
	values on the same line.
	Important note : As I don't fully understand what transfer-extension is and
	as I don't find any information on it outside of the RFCs I will consider
	it as something uncommon and that doesn't have to be take into account.
*/

bool	Value::_checkTransferEncoding(std::string &value)
{
	size_t pos = 0;
	size_t old_pos = 0;
	while (value[pos])
	{
		for (int i = 0; i < 4; i++)
		{
			if (!value.compare(pos, Value::_tcoding[i].length(), Value::_tcoding[i]))
			{
				pos += Value::_tcoding[i].length();
				size_t j = 0;
				for (j = 0; Header::_isSpace(value[pos+j]); j++) pos++;
				if (value[pos+j] == ',') pos++;
				for (j = 0; Header::_isSpace(value[pos+j]); j++) pos++;
				continue;
			}
		}
		if (pos == old_pos)
			return false;
		old_pos = pos;
	}
	return true;
}

/*
	Will return 'true' anytime since if the type or subtype is wrong the value will
	just be changed to an understandable form. Most of the time a wrong content-type
	will be transformed to "application/octet-stream" except if the type has already
	been acknowledged as "text/". If so the subtype will always be changed to "plain"
	if not recognized. 
	Important note : We won't check composite media types.
	Case sensitive? No
*/

bool	Value::_checkContentType(std::string &value)
{
	for (size_t i = 0; i < value.length(); i++)
		value[i] = std::tolower(value[i]);
	std::string const &type = Value::_getType(value);
	if (type == value)
		return true;
	else if (type == "" || value[type.length()] != '/')
	{
		value  = "application/octet-stream";
		return true;
	}
	std::string const &subtype = Value::_getSubtype(type, value.substr(type.length()+1));
	if (subtype == "" && type == "text")
		value = "text/plain";
	else if (subtype == "")
		value = "application/octet-stream";
	return true;
	//check potential parameter
}

/**/
	// Rules :
		// Accept = #( media-range [ accept-params ] )
		// media-range = ( "*/*"
		// / ( type "/" "*" )
		// / ( type "/" subtype )
		// ) *( OWS ";" OWS parameter )
		// accept-params = weight *( accept-ext )
		// accept-ext = OWS ";" OWS token [ "=" ( token / quoted-string ) ]
/*
	Case sensitive? No
	Note that there can be multiple values on the same line.
*/

bool	Value::_checkAccept(std::string &value)
{
	for (size_t i = 0; i < value.length(); i++)
		value[i] = std::tolower(value[i]);
	if (Value::_getAccept(value) == "")
		return false;
	size_t pos = 0;
	while ((pos = value.find(",")) != std::string::npos)
	{
		pos++;
		while (Header::_isSpace(value[pos]))
			pos++;
		if (Value::_getAccept(value.substr(pos)) == "")
			return false;
	}
	return true;
}

/*
	Case sensitive? No
*/

bool	Value::_checkExpect(std::string &value)
{
	for (size_t i = 0; i < value.length(); i++)
		value[i] = std::tolower(value[i]);
	if (value != "100-continue")
		return false;
	return true;
}

//WOULD TAKE HOURS//

/*
	From = mailbox
	mailbox = name-addr / addr-spec
	name-addr = [display-name] angle-addr
	display-name = phrase
	angle-addr = [CFWS] "<" addr-spec ">" [CFWS] /
 				obs-angle-addr
	addr-spec = local-part "@" domain
	local-part = dot-atom / quoted-string / obs-local-part
	domain = dot-atom / domain-literal / obs-domain
 	domain-literal = [CFWS] "[" *([FWS] dtext) [FWS] "]" [CFWS]
 	dtext = %d33-90 / ; Printable US-ASCII
 			%d94-126 / ; characters not including
 			obs-dtext ; "[", "]", or "\"
	obs-angle-addr = [CFWS] "<" obs-route addr-spec ">" [CFWS]
	dot-atom = [CFWS] dot-atom-text [CFWS]
	dot-atom-text = 1*atext *("." 1*atext)
	atext = ALPHA / DIGIT / "!" / "#" / "$" / "%" / "&" / "’" / "*" / "+" / "-" /
			"/" / "=" / "?" / "^" / "_" / "‘" / "{" / "|" / "}" / "˜"
	quoted-string = [CFWS]
 					DQUOTE *([FWS] qcontent) [FWS] DQUOTE
 					[CFWS]
	obs-local-part = word *("." word)
	obs-domain = atom *("." atom)
	obs-dtext = obs-NO-WS-CTL / quoted-pair
	obs-route = obs-domain-list ":"
	qcontent = qtext / quoted-pair
	word = atom / quoted-string
	quoted-pair = ("\" (VCHAR / WSP)) / obs-qp
	obs-domain-list = *(CFWS / ",") "@" domain *("," [CFWS] ["@" domain])
*/

// bool	Value::checkFrom(std::string const &value)
// {
	
// }