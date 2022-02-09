/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/08 16:52:44 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/09 11:40:46 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef URI_HPP
# define URI_HPP

#include "Header.hpp"

class URI
{
	public:
		URI(void);
		URI(URI const &src);
		~URI(void);

		URI	&operator=(URI const &right);

		std::string const	&getHTTPForm(void) const;
		std::string const	&getNormalizedForm(void) const;
		void				setHTTPForm(std::string const &str);
		void				setNormalizedForm(std::string const &str);
	private:
		enum hierPartType {AUTHORITY, ABSOLUTE, ROOTLESS, EMPTY, UNDEFINED};
		
		std::string _HTTPForm, _normalizedForm;
		std::string _scheme, _hierPart, _query, _fragment, _auth, _path;
		hierPartType 				_hpType;
		void						_uriFormating(void);
		void						_hierPartFormating(void);
		int const					_uriNormalization(void);
		static std::string const	_strNormalization(std::string &str, size_t pos,
									std::string const &except, bool pct);
};

#endif