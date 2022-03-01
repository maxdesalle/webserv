/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 11:48:07 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/17 17:30:46 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "Header.hpp"

class Response : public Header
{
	private:
		static std::string const _fieldNames[16];
		std::string		_body;
	public:
		Response(void);
		Response(Response const &src);
		~Response(void);

		Response const &operator=(Response const &right);

		void	bodyAppend(const std::string &buff);
};

#endif
