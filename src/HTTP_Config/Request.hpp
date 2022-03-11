/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 11:47:16 by ldelmas           #+#    #+#             */
/*   Updated: 2022/03/10 12:15:47 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Header.hpp"

# include <cstdlib>
# include <cerrno>

# include "URI.hpp"
# include "Value.hpp"
# include "../config/Config.hpp"
# include "../sockets/NetworkIPC.hpp"
# include "../utils/utils.hpp"

/*Number of CGI's environment variables*/
# define ENV_NUM 18
# define FIELDNAME_MAX 1024

/*
** TO DO
**	- update constructors with new member variables
*/

class Request : public Header
{
	public:
		enum state {STARTLINE, HEADERS, BODY, DONE};
		enum chunk {SIZE, DATA, TE}; // tderwedu
	private:
		enum body {NONE, LEN, CHUNKED};
	
		static std::string const _fieldNames[32];
		static std::string const _cgiSerVarNames[ENV_NUM];
		std::string		_method, _target, _version, _body, _remain;
		std::map<std::string const, std::string> _cgiSerVars;
		size_t			_cursor;
		size_t			_body_size; // tderwedu
		state			_state;
		body			_type;
		bool			_expect;
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

		Request 			&operator=(Request const &right);

		std::string const	&getMethod(void) const;
		std::string const	&getTarget(void) const;
		std::string const	&getVersion(void) const;
		std::string const	&getBody(void) const;
		state				getState(void) const;
		bool				isDone(void) const;
		std::map<std::string const, std::string> &getCGIServerVars(void);
		void				setMethod(std::string &method);
		void				setTarget(std::string &target);
		void				setVersion(std::string &version);
		void				setBody(std::string &body);
		void				setCGIServerVars(Location const &CGILocation, in_addr_t addr);

		int					parseRequest(std::string const &request);
		void				printRequest(void);

		void				reset(void); // tderwedu => state = STARTLINE, CLEAR map, _method, _target, _version, _body, _remain, ...

		friend std::ostream&	operator<<(std::ostream& stream, Request const& sock);
};

#endif
