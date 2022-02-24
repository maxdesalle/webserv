/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:57:33 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/24 19:39:51 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

RequestHandler::RequestHandler(): _request(), _response() {}
RequestHandler::RequestHandler(RequestHandler const& rhs) { *this = rhs; }

RequestHandler::~RequestHandler() {}

RequestHandler&	RequestHandler::operator=(RequestHandler const& rhs)
{
	if (this != &rhs)
	{
		_request = rhs._request;
		_response = rhs._response;
		_server = rhs._server;
		_location = rhs._location;
		_state = rhs._state;
	}
	return *this;
}

Request&			RequestHandler::getRequest(void)
{
	return _request;
}

void			RequestHandler::setServer(Server& server)
{
	_server = &server;
}

void				RequestHandler::clearRequestHandler(void)
{
	//TODO: to do ...
}

void			RequestHandler::processRessource(void)
{
	if (_state == NEW)
	{
		_findLocation(_request.getTarget());
		if (!_location)
		{
			_state = SENDING;
			;	// TODO: return 404 not found!
		}
		if (!_location->isMethodValid(_request.getMethod()))
		{
			_state = SENDING;
			;	// TODO: return 405 Method Not Allowed!
		}
		if (!_location->isCgi())
		{
			_state = CGI;
			;	// TODO: HANDLING CGI
		}
	}
}

void			RequestHandler::_findLocation(const std::string &path)			//TODO: CORR GetLocations
{
	int					len = 0;
	int					max = 0;
	const Location		*candidate = NULL;
	vecLocation			locations = _server->GetLocations();

	for (citLocation it = locations.begin(); it < locations.end(); ++it)
	{
		if (path.find(it->GetPath()) == 0)
		{
			len = (it->GetPath()).size();
			if (len > max)
			{
				candidate = &*it;
				max = len;
			}
		}
	}
	_location = candidate;
}
