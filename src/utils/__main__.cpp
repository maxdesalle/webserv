/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __main__.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 17:18:51 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/17 18:44:22 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.cpp"
#include <iostream>

int	main(void)
{
	std::string		buff;
	std::string		path = std::string(".");

	buff = get_autoindex(path);
	std::cout << buff << std::endl;
}
