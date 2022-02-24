/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Value.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 16:26:58 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/24 14:47:03 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Value.hpp"

Value::Value(void) {}

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
}

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


bool	Value::checkContentLength(std::string const &value)
{
	std::string length = Header::_parsePort(value);
	if (length == "" || length != value)
		return false;
	return true;
}

/*
	transfer-coding = "chunked" / "compress" / "deflate" / "gzip" / transfer-extension
	transfer-extension = token *( OWS ";" OWS transfer-parameter )
	transfer-parameter = token BWS "=" BWS ( token / quoted-string )
	quoted-string = DQUOTE *( qdtext / quoted-pair ) DQUOTE
	qdtext = HTAB / SP /%x21 / %x23-5B / %x5D-7E / obs-text
	obs-text = %x80-FF
	quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )
*/

bool	Value::checkTransferEncoding(std::string const &value)
{
	
}





//SHOULD BE "application/octet-stream" IF NOTHING IS GIVEN BY REQUEST
//FULL PARSING (erasing some spaces, everything go lowcase...)
// bool	Value::checkContentType(std::string const &value)
// {
// 	//WE NEED TOKENS FOR THIS ONE -> where do I find them?
// 	//EXAMPLE : text/plain
// }

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