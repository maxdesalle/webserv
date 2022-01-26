/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:31:16 by ldelmas           #+#    #+#             */
/*   Updated: 2022/01/26 15:06:41 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
# define HEADER_HPP

# include <string>
# include <map>

class Header
{
    protected:
		//common informations between requests and responses types of header
		std::map<std::string const, std::string> headerFields;
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