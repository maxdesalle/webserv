/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 14:58:24 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/04 14:04:15 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::string const Response::_fieldNames[16] = {"Age", "Cache-Control", "Expires", 
											"Date", "Location", "Retry-After",
											"Vary", "Warning", "ETag", "Last-Modified",
											"WWW-Authenticate", "Proxy-Authenticate",
											"Accept-Ranges", "Allow", "Server", ""};

Response::Response(void) : Header(Response::_fieldNames) {}

Response::Response(Response const &src) {this->_headerFields = src._headerFields;}

Response::~Response(void) {}

Response const &Response::operator=(Response const &right)
{
	this->_headerFields = right._headerFields;
	return *this;
}
