/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 11:47:16 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/02 16:47:15 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Header.hpp"

class Request : public Header
{
	private:
		static std::string const _fieldNames[22];
		std::string		_method, _target, _version, _body;
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