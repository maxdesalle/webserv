/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 10:57:33 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/23 10:57:56 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestHandler.hpp"

RequestHandler::RequestHandler() : _request(Request()){}

RequestHandler::~RequestHandler() {}

void			RequestHandler::setServer(Server& server)
{
	_server = &server;
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
	int						len = 0;
	int						max = 0;
	Location				*candidate = NULL;
	std::vector<Location>	locations = _server->GetLocations();

	for (std::vector<Location>::iterator it = locations.begin(); it < locations.end(); ++it)
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
