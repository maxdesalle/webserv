/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 11:19:44 by mdesalle          #+#    #+#             */
/*   Updated: 2022/02/23 15:06:27 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <string>
# include <iostream>
# include <cctype>
# include <cstdlib>
# include <fstream>
# include <algorithm>
# include <vector>
# include <map>

# include "Location.hpp"
# include "Server.hpp"

class Location;
class Server;

typedef std::vector<Server>									vecServer;
typedef std::vector<Server>::iterator						itServer;
typedef std::vector<Server>::const_iterator					citServer;
typedef std::vector<Location>								vecLocation;
typedef std::vector<Location>::iterator						itLocation;
typedef std::vector<Location>::const_iterator				citLocation;
typedef std::map<std::string, std::vector<size_t> >			mapServer;

std::vector<Server>		ConfigHandler(std::string ConfigFileName);
std::vector<Server>		*FindMatchingServers(std::vector<Server> &Servers, size_t Port, std::string IP);
void					printServers(std::vector<Server> &Servers);

#endif
