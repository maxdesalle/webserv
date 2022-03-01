/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 16:52:44 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/23 16:34:54 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URI_HPP
# define URI_HPP

#include "Header.hpp"
#include "Request.hpp"

/*
	Description of the URI class :
	
	Purpose :
	Take a string as an argument from construction. If this string is
	a correctly written URI it will be "normalized" as described in the [RFC3986].
	The "brute" version of the string is stored in the _HTTPForm attribute while
	the "normalized" version is stored in the _normalizedForm attribute.
	
	Attributes :
	The strings _scheme, _hierPart, _query and _fragment are substrings
	composing a URI. The query and the fragment being facultative they might be
	empty strings during the whole process. The strings _auth and _path can be
	substrings composing a _hierPart in some cases. All of those substrings have
	different syntaxes and will be normalized differently.
	The _hpType will give the type of hier-part composing the URI which is
	useful for the noralization. The _status type explain how the _HTTPForm is
	considered by the class as a URI.

	Methods :
	The methods are private and the user should only call the class with
	its string constructor to get the normalization of the URI. The formating
	methods stock the substrings of the URI in their corresponding attributes.
	The normalization methods transform the substring depending on their specific
	syntax then concatenate them to create a new normalized URI.
	The _strNormalization method is a static method taking a string as an argument
	and returning its normalized form.

	Important note :
	For a fast formating and normalization the URI class expect the user to
	give a URI already correctly written. Any attempt to give a URI with wrong
	syntax will lead to a Header::WrongSyntaxException. In some unexpected case
	it might even return a wrong URI in the _normalizedForm attribute.

	Example of use :
	#################################################################################################
	#	//The whole process of normalization is done in the constructor//							#
	#	URI my_uri("BLA+-.NLE://LKJ%7E::45%286!$&::@[45FD:78AB::]:123456/%7E/ALKJEL/bonjour%27");	#
	#	if (my_uri.getStatus() == "NORMALIZED")														#
	#		std::cout << my_uri.getNormalizedForm() << std::endl;									#
	#	else if (my_uri.getStatus() == "WRONG_SYNTAX")												#
	#		std::cout << "Wrong syntax" << std::endl;												#
	#################################################################################################
	Output expected : "bla+-.nle://lkj~::45(6!$&::@[45FD:78AB::]:123456/~/alkjel/bonjour'"
*/


class URI
{
	enum uristatus {NORMALIZED, WRONG_SYNTAX, UNCHECKED};
	enum hierPartType {AUTHORITY, ABSOLUTE, ROOTLESS, EMPTY, UNDEFINED};

	public:
		URI(void);
		URI(std::string const &httpURI);
		URI(URI const &src);
		~URI(void);

		URI	&operator=(URI const &right);

		std::string const	&getHTTPForm(void) const;
		std::string const	&getNormalizedForm(void) const;
		std::string const	getStatus(void) const;
		void				setHTTPForm(std::string const &str);
		void				setNormalizedForm(std::string const &str);
	private:
		
		std::string _HTTPForm, _normalizedForm;
		std::string _scheme, _hierPart, _query, _fragment, _auth, _path;
		hierPartType 				_hpType;
		uristatus					_status;
		void						_uriFormating(void);
		void						_hierPartFormating(void);
		int							_authNormalization(void);
		int							_uriNormalization(void);

		static std::string const	_strNormalization(std::string &str, size_t pos,
									std::string const &except, bool pct);
};

#endif
