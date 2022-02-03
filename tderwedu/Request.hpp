/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 11:47:16 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/03 15:44:27 by tderwedu         ###   ########.fr       */
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
	public:
		enum Body {LEN, CHUNKED, NONE};
		enum State {NONE, DOWNLOADING, DOWNLOADED, PROCESSING, DONE};
		
	private:
		static std::string const _fieldNames[22];
		std::string		_method, _target, _version, _body;
		Body			_type;
		State			_state;
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
		void				setBody(std::string &Body);

		int					parseRequest(std::string const &request);
		int					parseRequestLine(std::string const &r_line);
		int					parseHeaderField(std::string const &line);

		// Added by tderwedu
		void				move(Request& src);
		void				clear(void);
		void				stopRequest(void);
		State				getState(void);
		Body				getBodyType(void);
};

#endif
