/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Value.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 16:12:03 by ldelmas           #+#    #+#             */
/*   Updated: 2022/03/01 11:10:39 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALUE_HPP
# define VALUE_HPP

# include <string>
# include <map>
# include "Header.hpp"

# define NTYPE 5
# define NTEXT 1
# define NIMG 5
# define NAUDIO 6
# define NVIDEO 2
# define NAPP 2

/*
	Description of the Value class :

	Purpose :
	With its only public method this class check if a key-value is syntaxically
	correct given its key-name. This version is way more simplified compared to
	the RFCs description.
	Everything is static in this class since there is no advantage of instantiations.
	The constructors and constructor are useless and are only here to correspond to
	the canonical form.

	Attributes :
	The arrays of strings private attributes correspond to token that can be found
	in the key values. For example Value::_types has the different existing
	media-types described in [RFC7231]. A map Value::_stypes contains arrays of
	subtype strings with the key being the corresponding type name.
	The Value::_checkers contains every private method corresponding the key-names
	of header fields. This way Value::_checkers["Host"](value) will tell us if the
	string contained in 'value' is a correctly written Host.

	Methods :
	The methods are static and private and are used to check if the key-value are
	correct. Some value can be changed depending on the context (going lowcase,
	replaced by standard value...).
	The only public method is Value::checkFieldValue(name, value) and will return
	a bool depending on the name (give the syntaxic rule) and the value (is checked
	to see if syntaxically correct).

	Important note :
	For now the only key-names we're managing are :
		-Host
		-Content-Type
		-Content-Length
		-Accept
		-Expect
		-Transfer-Encoding
	and their management is much more basic than described in the RFCs as the purpose
	of the exercice is to build a "simple" server in C++.

	Example of use A :
	#################################################################################################
	#	std::string name = "Content-Type";															#
	#	std::string val_1 = "text/plain";															#
	#	std::string val_2 = "wrong";																#
	#	if (Value::checkFieldValue(name, val_1))													#
	#		std::cout << val_1 << " is a correct Content-Type!" << std::endl;						#
	#	if (Value::checkFieldValue(name, val_2))													#
	#		std::cout << val_2 << " is a value if a wrong Content-Type is given!" << std::endl;		#
	#################################################################################################
	Expected output :	"text/plain is a correct Content-Type!"
						"application/octet-stream is a value if a wrong Content-Type is given!"
	
	Example of use B :
	#################################################################################################
	#	std::string name = "Expect";																#
	#	std::string val_1 = "100-continue";															#
	#	std::string val_2 = "wrong";																#
	#	if (Value::checkFieldValue(name, val_1))													#
	#		std::cout << val_1 << " is a correct Expect!" << std::endl;								#
	#	if (!Value::checkFieldValue(name, val_2))													#
	#		std::cout << val_2 << " is a wrong Expect!" << std::endl;								#
	#################################################################################################
	Expected output :	"100-continue is a correct Expect!"
						"wrong is is a wrong Expect!"
*/

class Value
{
	typedef	std::pair<std::string const, bool(&)(std::string &value)> valPair;
	typedef std::pair<std::string const, std::string const *> subPair;

	public:
		Value(void);
		Value(Value const &src);
		~Value(void);

		Value &operator=(Value const &src);

		static bool checkFieldValue(std::string const &name, std::string &value);

	private:
		static const	std::string	_tcoding[4];
		static const	std::string	_types[NTYPE];
		static const	std::string _sub_text[NTEXT];
		static const	std::string _sub_image[NIMG];
		static const	std::string _sub_audio[NAUDIO];
		static const	std::string _sub_video[NVIDEO];
		static const	std::string _sub_app[NAPP];
		static const	std::string *_stypes[NTYPE];
		static std::map<std::string const, std::string const*>			_subtypes;
		static std::map<std::string const, int const>					_num_stypes;
		static std::map<std::string const, bool(&)(std::string &value)>	_checkers;

		static void					_initialize(void);

		static std::string const	_getType(std::string const &value);
		static std::string const	_getSubtype(std::string const &type, std::string const &value);
		static std::string const	_getAccept(std::string const &value);

		static bool _checkHost(std::string &value);
		static bool _checkContentType(std::string &value);
		static bool _checkAccept(std::string &value);
		static bool _checkContentLength(std::string &value);
		static bool _checkTransferEncoding(std::string &value);
		static bool _checkExpect(std::string &value);
		// static bool checkFrom(std::string const &value);
};

#endif
