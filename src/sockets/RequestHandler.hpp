/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 11:49:23 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/14 15:49:00 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include "Server.hpp"
# include "Location.hpp"
# include "Request.hpp"
# include "Response.hpp"

class RequestHandler
{
private:
	const Request&		_request;
	const Server&		_server;
	const Location&		_location;
	const Response&		_response;
public:
	RequestHandler(/* args */);
	~RequestHandler();

	Request&		getRequest(void);
	void			setServer(const Server& server);
};

RequestHandler::RequestHandler(/* args */)
{
}

RequestHandler::~RequestHandler()
{
}



#endif
