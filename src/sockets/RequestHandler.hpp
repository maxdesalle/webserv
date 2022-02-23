/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 11:49:23 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/23 10:58:01 by tderwedu         ###   ########.fr       */
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

#endif
