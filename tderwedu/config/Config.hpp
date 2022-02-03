/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesalle <mdesalle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 11:19:44 by mdesalle          #+#    #+#             */
/*   Updated: 2022/02/02 17:09:54 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "../../include/Webserv.hpp"
#include "Location.hpp"
#include "Server.hpp"

#include <fstream>

class Server;

std::vector<Server>	ConfigHandler(std::string ConfigFileName);

#endif
