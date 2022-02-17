/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:31:19 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/17 12:12:34 by tderwedu         ###   ########.fr       */
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



std::string		getLastModified(const std::string &path)
{
	char	timestamp[30];
	//TODO: to do
}

std::string		getIMFfixdate(const time_t *timestamp)
{
	char		buff[30];
	struct tm	*date; // statically allocate struct

	date = gmtime(timestamp);
	strftime(buff, 30, "%a, %d %b %Y %T GMT", date);
	return std::string(buff);
}

bool is_file(const std::string &path)
{
	struct stat s;
}

bool is_dir(const std::string &path)
{
	
}
