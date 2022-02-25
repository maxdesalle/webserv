/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Value.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 16:26:58 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/25 16:35:37 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Value.hpp"

const std::string Value::_tcoding[4] = {"chunked", "compress", "deflate", "gzip"};
const std::string Value::_types[5] = {"text", "image", "audio", "video", "application"};
const std::string Value::_sub_text[1] = {"plain"}; //anything unrecognized is text/plain if known charset
const std::string Value::_sub_image[5] = {"jpeg", "png", "gif", "bmp", "webp"};
const std::string Value::_sub_audio[6] = {"basic", "midi", "wav", "mpeg", "webm", "ogg"};
const std::string Value::_sub_video[2] = {"webm", "ogg"};
const std::string Value::_sub_app[2] = {"octet-stream", "postscript"};

Value::Value(void)
{
	
}

Value::Value(Value const &src)
{
	*this = src;
}

Value::Value(Request const &request)
{
	this->_setCheckers(request);
}

Value::~Value(void) {};

void	Value::_setCheckers(Request const &request)
{
	this->_checkers.insert(valPair("Host", checkHost));
	this->_checkers.insert(valPair("Content-Length", checkContentLength));
	this->_checkers.insert(valPair("Transfer-Encoding", checkTransferEncoding));
}

/*
	Case sensitive? Only IPs and reg-name -> not our problem.
*/

bool	Value::checkHost(std::string const &value)
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

bool	Value::checkContentLength(std::string const &value)
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

bool	Value::checkTransferEncoding(std::string const &value)
{
	size_t pos = 0;
	while (value[pos])
	{
		for (int i = 0; i < 4; i++)
		{
			if (!value.compare(pos, Value::_tcoding[i].length(), Value::_tcoding[i]))
			{
				pos += Value::_tcoding[i].length();
				size_t j = 0;
				for (j = 0; value[pos+j]; j++) pos++;
				if (value[pos+j] == ',') pos++;
				for (j = 0; value[pos+j]; j++) pos++;
				continue;
			}
		}
		return false;
	}
	return true;
}

// static int	checkParameter(std::string &par)
// {
// 	std::string::const_iterator it = par.begin();
// 	for (int i = 0; i < par.length()&&(par[i]==' '||par[i]=='\t'); i++) it++;
	
// }

/*
	Important note : We won't check composite media types.

	SHOULD BE "application/octet-stream" IF NOTHING IS GIVEN BY REQUEST
	FULL PARSING (erasing some spaces, everything go lowcase...)
	WE NEED TOKENS FOR THIS ONE -> where do I find them?
	EXAMPLE : text/plain
*/
bool	Value::checkContentType(std::string &value)
{
	for (size_t i = 0; i < value.length(); i++)
		value[i] = std::tolower(value[i]);
	int i = 0;
	size_t len;
	for (i = 0; i < 5; i++)
	{
		len = Value::_types[i].length();
		if (!value.compare(0, len, Value::_types[i]))
			break ;
	}
	if (i == 5 || value[Value::_types[i].length()] != '/')
		return false;
	//check the subtypes
	//check potential parameter
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