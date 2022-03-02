/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 11:49:23 by tderwedu          #+#    #+#             */
/*   Updated: 2022/03/02 12:49:33 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include "NetworkIPC.hpp"

class RequestHandler
{
public:
	enum State {NEW, DOWNLOADED, SENDING, DONE};
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
	Response&			getResponse(void);
	Location const&		getLocation(void);
	
	void				setStateDownloaded(void);
	void				setStateSending(void);
	void				setStateDone(void);
	bool				isStateDownloaded(void);
	bool				isStateSending(void);
	bool				isStateDone(void);

	std::string			getHost(void); // TODO
	std::string			getRoot(void); // TODO
	std::string			getTarget(void); // TODO
	std::string			getPath(void); // TODO

	void				clearRequestHandler(void);
	void				processRessource(void);


	void				setServer(Server const& server);
	void				_findLocation(const std::string &path);
};

#endif
