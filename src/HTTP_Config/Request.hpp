/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 11:47:16 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/14 17:36:43 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Header.hpp"
# include "URI.hpp"

/*
** TO DO
**	- update constructors with new member variables
*/

class Request : public Header
{
	public:
		enum state {STARTLINE, HEADERS, BODY, PROCESSING, DONE};
	private:
		enum body {LEN, CHUNKED, NONE};
		
		static std::string const _fieldNames[32];
		static std::string const _cgiSerVarNames[19];
		std::string		_method, _target, _version, _body, _remain;
		std::map<std::string const, std::string> _cgiSerVars;
		size_t			_cursor;
		body			_type;
		state			_state;

		int				_getNextLine(std::string const &str, std::string &line);
		int				_parseRequestLine(std::string const &r_line);
		int				_parseHeaderField(std::string const &line);

	public:
		Request(void);
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
		void				setCGIServerVars(void);

		int					parseRequest(std::string const &request);
};

#endif
