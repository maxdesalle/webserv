/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:57:33 by tderwedu          #+#    #+#             */
/*   Updated: 2022/03/02 13:18:35 by tderwedu         ###   ########.fr       */
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

Response&			RequestHandler::getResponse(void)
{
	return _response;
}

Location const&			RequestHandler::getLocation(void)
{
	return *_location;
}

void			RequestHandler::setServer(Server const& server)
{
	_server = &server;
}

void			RequestHandler::setStateDownloaded(void)
{
	_state = DOWNLOADED;
}

void			RequestHandler::setStateSending(void)
{
	_state = SENDING;
}

void			RequestHandler::setStateDone(void)
{
	_state = DONE;
}

bool			RequestHandler::isStateDownloaded(void)
{
	return _state == DOWNLOADED;
}

bool			RequestHandler::isStateSending(void)
{
	return _state == DOWNLOADED;
}

bool			RequestHandler::isStateDone(void)
{
	return _state == DONE;
}


void				RequestHandler::clearRequestHandler(void)
{
	//TODO: to do ...
}

void			RequestHandler::processRessource(void) // TODO:remove
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
	}
}

void			RequestHandler::_findLocation(const std::string &path)			//TODO: CORR GetLocations
{
	int					len = 0;
	int					max = 0;
	Location const		*candidate = NULL;
	vecLocation const&	locations = _server->GetLocations();
	std::string			loc;

	for (citLocation it = locations.begin(); it < locations.end(); ++it)
	{
		loc = it->GetPath();
		if (path.find(loc) == 0)
		{
			len = (loc).size();
			if (len > max)
			{
				candidate = &*it;
				max = len;
			}
		}
	}
	_location = candidate;
	if (!candidate)
		; //TODO: error 404 !!!
	std::cout << "  SERVER\n"  << *_server << std::endl; // TODO:remove
	std::cout << "LOCATION\n" << *_location << std::endl;
}
