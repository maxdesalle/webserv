/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigHandler.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxdesalle <mdesalle@student.s19.be>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/18 18:04:43 by maxdesall         #+#    #+#             */
/*   Updated: 2022/01/19 17:11:31 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

/*
 * main config function which handles the whole config process which includes
 * reading from the config file, checking the read information and saving it
 * in data structures
 */

void	ConfigHandler(char *file_name)
{
	ConfigReader	config(file_name);
	/* for (size_t i = 0; i < config.GetConfigFileContent().size(); i += 1) */
	/* 	std::cout << config.GetConfigFileContent()[i] << std::endl; */
}
