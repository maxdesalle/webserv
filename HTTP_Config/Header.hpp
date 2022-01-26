/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:31:16 by ldelmas           #+#    #+#             */
/*   Updated: 2022/01/26 11:50:35 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
# define REQUEST_HPP

# include <string>
# include <map>

class Header
{
    protected:
		//common informations between requests and responses types of header
		std::map<std::string, std::string> headerFields;
    public:
        Header(void);
        Header(Header const &src);
        ~Header(void);

        Header const &operator=(Header const &right);
};

# endif