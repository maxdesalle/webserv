/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/22 10:21:42 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/22 10:40:28 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <algorithm>


int main(void)
{
	std::string		input = "error_page";
	static const std::string		SpecialKeys[] = { "return", "error_page" };

	for (size_t i = 0; i < sizeof(SpecialKeys)/ sizeof(std::string); ++i)
	{
		if (input == SpecialKeys[i])
			std::cout << "FOUND! : " << SpecialKeys[i] << std::endl;
	}

	std::cout << "=== ===" << std::endl;

	std::string		input2 = "  error_page  ";
	std::string::iterator it = std::remove(input2.begin(), input2.end(), ' ');
	std::cout << "REMOVED: " << *it << std::endl;
}
