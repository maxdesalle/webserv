/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:31:16 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/24 10:20:43 by ldelmas          ###   ########.fr       */
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

		std::map<std::string const, std::string> _headerFields;

	public:
		static std::string const alpha, digit, unreserved, sub_delims,
								gen_delims, reserved, hexdig, custom_pchar;
		static std::string const authorizedMethods[4];
		/*
			All of the protected methods used to parse the request
		*/
		static bool const			_isSpace(unsigned char c);
		static bool const			_isPrintable(unsigned char c);
		static bool const			_isObsText(unsigned char c);
		static bool const			_isFieldVchar(unsigned char c);
		static std::string const	_parseHTTPVersion(std::string const &str, size_t pos=0);
		static std::string const	_parseMethod(std::string const &str, size_t pos=0);
		static std::string const	_parsePctEncoded(std::string const &str, size_t pos=0);
		static std::string const	_parsePchar(std::string const &str, size_t pos=0);
		static std::string const	_parseSegment(std::string const &str, size_t pos=0);
		static std::string const	_parseSegmentNz(std::string const &str, size_t pos=0);
		static std::string const	_parseQuery(std::string const &str, size_t pos=0);
		static std::string const	_parsePathAbempty(std::string const &str, size_t pos=0);
		static std::string const	_parseAbsPath(std::string const &str, size_t pos=0);
		static std::string const	_parsePathRootless(std::string const &str, size_t pos=0);
		static std::string const	_parsePathAbs(std::string const &str, size_t pos=0);
        static std::string const	_parseScheme(std::string const &str, size_t pos=0);
		static std::string const	_parseUserInfo(std::string const &str, size_t pos=0);
		static std::string const	_parseRegName(std::string const &str, size_t pos=0);
		static std::string const	_parsePort(std::string const &str, size_t pos=0);
		static std::string const	_parseDecOctet(std::string const &str, size_t pos=0);
		static std::string const	_parseIpv4Address(std::string const &str, size_t pos=0);
		static std::string const	_parseHexa16(std::string const &str, size_t pos=0);
		static std::string const	_parseLow32(std::string const &str, size_t pos=0);
		static std::string const	_parseIpv6Address(std::string const &str, size_t pos=0);
		static std::string const	_parseIpvFuture(std::string const &str, size_t pos=0);
		static std::string const	_parseIpLiteral(std::string const &str, size_t pos=0);
		static std::string const	_parseHost(std::string const &str, size_t pos=0);
		static std::string const	_parseAuthority(std::string const &str, size_t pos=0);
		static std::string const	_parseHierPart(std::string const &str, size_t pos=0);
		static std::string const	_parseOriginForm(std::string const &str, size_t pos=0);
		static std::string const	_parseAbsForm(std::string const &str, size_t pos=0);
		static std::string const	_parseRequestTarget(std::string const &str, size_t pos=0);
		static std::string const	_parseRequestTarget(std::string &type, std::string const &str, size_t pos=0);
		static std::string const	_parseURI(std::string const &str, size_t pos=0);
		static std::string const	_parseObsFold(std::string const &str, size_t pos=0);
		static std::string const	_parseFieldContent(std::string const &str, size_t pos=0);
		static std::string const	_parseFieldValue(std::string const &str, size_t pos=0);
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
		bool const			findField(std::string const fieldName) const;
};

# endif