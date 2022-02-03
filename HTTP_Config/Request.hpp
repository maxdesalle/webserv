/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 11:47:16 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/03 10:01:10 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Header.hpp"

/*
** TO DO
**	- update constructors with new member variables
*/

class Request : public Header
{
	private:
		enum body {LEN, CHUNKED, NONE};
		enum state {STARTLINE, HEADERS, BODY, PROCESSING, DONE};
		
		static std::string const _fieldNames[22];
		std::string		_method, _target, _version, _body;
		body			_type;
		state			_state;
	public:
		Request(void);
		Request(Request const &src);
		~Request(void);

		Request const		&operator=(Request const &right);

		std::string const	&getMethod(void) const;
		std::string const	&getTarget(void) const;
		std::string const	&getVersion(void) const;
		std::string const	&getBody(void) const;
		void				setMethod(std::string &method);
		void				setTarget(std::string &target);
		void				setVersion(std::string &version);
		void				setBody(std::string &body);

		int					parseRequest(std::string const &request);
		int					parseRequestLine(std::string const &r_line);
		int					parseHeaderField(std::string const &line);
};

#endif
