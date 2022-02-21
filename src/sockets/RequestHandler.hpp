/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 11:49:23 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/17 17:47:34 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include "../config/Server.hpp"
# include "../config/Location.hpp"
# include "../HTTP_Config/Request.hpp"
# include "../HTTP_Config/Response.hpp"

class RequestHandler
{
public:
	enum State {NEW, CGI, SENDING};
private:
	const Request&		_request;
	Server const		*_server;
	Location const		*_location;
	Response			*_response;
	State				_state;
public:
	RequestHandler();
	~RequestHandler();

	Request&		getRequest(void);

	void			setServer(Server& server);
	void			clearRequestHandler(void);
	void			processRessource(void);

	std::string		getHost(void); // TODO
	std::string		getRoot(void); // TODO
	std::string		getTarget(void); // TODO
	std::string		getPath(void); // TODO

	void			_findLocation(const std::string &path);
};

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

#endif
