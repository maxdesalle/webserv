/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:31:16 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/02 17:10:27 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
# define HEADER_HPP

# include <string>
# include <iostream>
# include <map>
# include <algorithm>
# include <stdexcept>


class Header
{
    protected:
		/*
			common informations between requests and responses types of header
		*/
		static std::string const _alpha, _digit, _unreserved, _sub_delims,
								_hexdig, _custom_pchar;
		static std::string const _authorizedMethods[4];

		std::map<std::string const, std::string> _headerFields;

		/*
			All of the protected methods used to parse the request line
			WARNING : I DIDN'T TEST ALL OF THEM YET!
		*/
		static std::string	_parseHTTPVersion(std::string &str, size_t pos=0);
		static std::string	_parseMethod(std::string &str, size_t pos=0);
		static std::string	_parsePctEncoded(std::string &str, size_t pos=0);
		static std::string	_parsePchar(std::string &str, size_t pos=0);
		static std::string	_parseSegment(std::string &str, size_t pos=0);
		static std::string	_parseSegmentNz(std::string &str, size_t pos=0);
		static std::string	_parseQuery(std::string &str, size_t pos=0);
		static std::string	_parsePathAbempty(std::string &str, size_t pos=0);
		static std::string	_parseAbsPath(std::string &str, size_t pos=0);
		static std::string	_parsePathRootless(std::string &str, size_t pos=0);
		static std::string	_parsePathAbs(std::string &str, size_t pos=0);
        static std::string	_parseScheme(std::string &str, size_t pos=0);
		static std::string	_parseUserInfo(std::string &str, size_t pos=0);
		static std::string	_parseRegName(std::string &str, size_t pos=0);
		static std::string	_parsePort(std::string &str, size_t pos=0);
		static std::string	_parseDecOctet(std::string &str, size_t pos=0);
		static std::string	_parseIpv4Address(std::string &str, size_t pos=0);
		static std::string	_parseHexa16(std::string &str, size_t pos=0);
		static std::string	_parseLow32(std::string &str, size_t pos=0);
		static std::string	_parseIpv6Address(std::string &str, size_t pos=0);
		static std::string	_parseIpvFuture(std::string &str, size_t pos=0);
		static std::string	_parseIpLiteral(std::string &str, size_t pos=0);
		static std::string	_parseHost(std::string &str, size_t pos=0);
		static std::string	_parseAuthority(std::string &str, size_t pos=0);
		static std::string	_parseHierPart(std::string &str, size_t pos=0);
		static std::string	_parseOriginForm(std::string &str, size_t pos=0);
		static std::string	_parseAbsForm(std::string &str, size_t pos=0);
		static std::string	_parseRequestTarget(std::string &str, size_t pos=0);

	public:
		class WrongSyntaxException : public std::exception
		{
			public :
				virtual const char *what(void) const throw();
		};


		Header(void);
		Header(std::string const *fieldNames);
        Header(Header const &src);
        ~Header(void);

        Header const &operator=(Header const &right);

		std::string const	getField(std::string const fieldName);
		void				setField(std::string const fieldName, std::string fieldValue);
};

# endif