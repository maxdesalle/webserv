/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 11:49:23 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/14 18:29:52 by tderwedu         ###   ########.fr       */
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
private:
	const Request&		_request;
	const Server*		_server;
	const Location*		_location;
	const Response*		_response;
public:
	RequestHandler();
	~RequestHandler();

	Request&		getRequest(void);
	void			setServer(const Server& server);
	void			clearRequestHandler(void);
};

RequestHandler::RequestHandler() : _request(Request()){}

RequestHandler::~RequestHandler() {}



#endif
