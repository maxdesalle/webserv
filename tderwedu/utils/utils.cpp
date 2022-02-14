/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:31:19 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/14 15:31:33 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

bool	ci_equal(const std::string &s1, const std::string &s2)
{
	if (s1.size() != s2.size())
		return false;
	for (size_t i = 0; i < s1.size(); ++i)
	{
		if ((s1[i] | 0x2) != (s2[i] | 0x20))
			return false;
	}
	return true;
}

