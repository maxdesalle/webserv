/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 11:49:23 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/24 19:38:12 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include "NetworkIPC.hpp"

class RequestHandler
{
public:
	enum State {NEW, CGI, SENDING};
private:
	Request				_request;
	Response			_response;
	Server const		*_server;
	Location const		*_location;
	State				_state;

public:
	RequestHandler();
	RequestHandler(RequestHandler const& rhs);
	virtual ~RequestHandler();
	RequestHandler&		operator=(RequestHandler const& rhs);

	Request&			getRequest(void);

	void				setServer(Server& server);
	void				clearRequestHandler(void);
	void				processRessource(void);

	std::string			getHost(void); // TODO
	std::string			getRoot(void); // TODO
	std::string			getTarget(void); // TODO
	std::string			getPath(void); // TODO

	void				_findLocation(const std::string &path);
};

#endif
