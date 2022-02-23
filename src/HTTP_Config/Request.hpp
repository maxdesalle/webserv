/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 11:47:16 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/23 16:29:55 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Header.hpp"

# include <cstdlib>
# include <cerrno>

# include "URI.hpp"
# include "../config/Location.hpp"
# include "../sockets/ClientSocket.hpp"

/*Number of CGI's environment variables*/
# define ENV_NUM 18
# define NAME_MAX 100

/*
** TO DO
**	- update constructors with new member variables
*/

class Request : public Header
{
	public:
		enum state {STARTLINE, HEADERS, BODY, PROCESSING, DONE};
		enum chunk {SIZE, DATA, TE}; // tderwedu
	private:
		enum body {LEN, CHUNKED, NONE};
	
		static std::string const _fieldNames[33];
		static std::string const _cgiSerVarNames[ENV_NUM];
		std::string		_method, _target, _version, _body, _remain;
		std::map<std::string const, std::string> _cgiSerVars;
		size_t			_cursor;
		size_t			_body_size; // tderwedu
		body			_type;
		state			_state;
		chunk			_chunk; // tderwedu

		int				_getNextLine(std::string const &str, std::string &line);
		int				_getNextField(std::string const &str, std::string &line);
		int				_parseRequestLine(std::string const &r_line);
		int				_parseHeaderField(std::string const &request);

		int				_findBodyType(void); // tderwedu
		int				_getBody(std::string const &buff); // tderwedu
	public:
		Request(void);
		Request(std::string const &request);
		Request(Request const &src);
		~Request(void);

		Request const		&operator=(Request const &right);

		std::string const	&getMethod(void) const;
		std::string const	&getTarget(void) const;
		std::string const	&getVersion(void) const;
		std::string const	&getBody(void) const;
		state				getState(void) const;
		std::map<std::string const, std::string> const &getCGIServerVars(void);
		void				setMethod(std::string &method);
		void				setTarget(std::string &target);
		void				setVersion(std::string &version);
		void				setBody(std::string &body);
		void				setCGIServerVars(Location &CGILocation, ClientSocket &Client);

		int					parseRequest(std::string const &request);

		void				reset(void); // tderwedu => state = STARTLINE, CLEAR map, _method, _target, _version, _body, _remain, ...
};

#endif
