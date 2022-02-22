/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 11:19:44 by mdesalle          #+#    #+#             */
/*   Updated: 2022/02/22 15:29:23 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "../../include/Webserv.hpp"
# include "Location.hpp"
# include "Server.hpp"

# include <cctype>
# include <cstdlib>
# include <fstream>
# include <algorithm>

class Server;

std::vector<Server>		ConfigHandler(std::string ConfigFileName);
std::vector<Server>		*FindMatchingServers(std::vector<Server> &Servers, size_t Port, std::string IP);
void					printServers(std::vector<Server> &Servers);
#endif
