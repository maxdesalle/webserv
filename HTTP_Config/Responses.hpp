/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Responses.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 11:48:07 by ldelmas           #+#    #+#             */
/*   Updated: 2022/01/26 11:48:25 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "Header.hpp"

class Response : public Header
{
	public:
		Response(void);
		Response(Response const &src);
		~Response(void);

		Response const &operator=(Response const &right);
};

#endif