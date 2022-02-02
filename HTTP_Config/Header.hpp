/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:31:16 by ldelmas           #+#    #+#             */
/*   Updated: 2022/01/31 15:47:41 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
# define HEADER_HPP

# include <string>
# include <iostream>
# include <map>
# include <algorithm>


class Header
{
    protected:
		//common informations between requests and responses types of header
		static std::string const _alpha, _digit, _unreserved, _sub_delims,
								_hexdig, _custom_pchar;
		static std::string const _authorizedMethods[4];
		
		std::map<std::string const, std::string> headerFields;

		static std::string	_parseHTTPVersion(std::string &str, size_t pos=0);
		static std::string	_parseMethod(std::string &str, size_t pos=0);
		static std::string	_parsePchar(std::string &str, size_t pos=0);
		static std::string	_parseSegment(std::string &str, size_t pos=0);
		static std::string	_parseQuery(std::string &str, size_t pos=0);
		static std::string	_parseAbsPath(std::string &str, size_t pos=0);
	public:
        Header(void);
		Header(std::string const *fieldNames);
        Header(Header const &src);
        ~Header(void);

        Header const &operator=(Header const &right);

		std::string const	getField(std::string const fieldName);
		void				setField(std::string const fieldName, std::string fieldValue);
};

# endif