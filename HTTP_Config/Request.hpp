/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 11:47:16 by ldelmas           #+#    #+#             */
/*   Updated: 2022/01/26 11:49:30 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Header.hpp"

class Request : public Header
{
	private:
		std::map<std::string, std::string> headerFields;
	public:
		Request(void);
		Request(Request const &src);
		~Request(void);

		Request const &operator=(Request const &right);
};

#endif