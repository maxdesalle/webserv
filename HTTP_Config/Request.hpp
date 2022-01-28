/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 11:47:16 by ldelmas           #+#    #+#             */
/*   Updated: 2022/01/28 14:04:21 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Header.hpp"

class Request : public Header
{
	private:
		static std::string const _fieldNames[22];
	public:
		Request(void);
		Request(Request const &src);
		~Request(void);

		Request const	&operator=(Request const &right);

		int				parseRequest(std::string const &request);
		int				parseRequestLine(std::string const &line);
};

#endif