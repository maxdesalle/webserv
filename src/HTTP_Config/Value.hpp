/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Value.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 16:12:03 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/24 14:27:12 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALUE_HPP
# define VALUE_HPP

#include "Request.hpp"

class Value
{
	typedef	std::pair<std::string const, bool(&)(std::string const &value)> valPair;

	public:
		Value(void);
		Value(Value const &src);
		Value(Request const &request);
		~Value(void);

		Value &operator=(Value const &src);

		bool checkFieldValue(std::string const &key);

	private:
		static const	std::string types[7];
		static const	std::string sub_text[4];
		static const	std::string sub_image[5];
		static const	std::string sub_audio[5];
		static const	std::string sub_video[2];
		static const	std::string sub_app[1];
		std::map<std::string const, bool(&)(std::string const &value)>	_checkers;

		void _setCheckers(Request const &request);

		static bool checkHost(std::string const &value);
		// static bool checkContentType(std::string const &value);
		static bool checkContentLength(std::string const &value);
		// static bool checkFrom(std::string const &value);
		static bool checkTransferEncoding(std::string const &value);
};

#endif
