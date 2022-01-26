/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 14:59:17 by ldelmas           #+#    #+#             */
/*   Updated: 2022/01/26 16:08:24 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Request.hpp"

std::string const Request::_fieldNames[22] = {"Cache-Control", "Expect", "Host", 
										"Max-Forwards", "Pragma", "Range",
										"TE", "If-Match", "If-None-Match",
										"If-Modified-Since", "If-Unmodified-Since",
										"If-Range", "Accept", "Accept-Charset",
										"Accept-Encoding", "Accept-Language",
										"Authorization", " Proxy-Authorization"
										"From", "Referer", "User-Agent", ""};

Request::Request(void) : Header(Request::_fieldNames) {}

Request::Request(Request const &src) {this->headerFields = src.headerFields;}

Request::~Request(void) {}

Request const &Request::operator=(Request const &right)
{
	this->headerFields = right.headerFields;
	return *this;
}