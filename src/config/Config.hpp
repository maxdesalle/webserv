/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 11:19:44 by mdesalle          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/03/02 19:02:49 by mdesalle         ###   ########.fr       */
=======
/*   Updated: 2022/03/02 18:40:29 by tderwedu         ###   ########.fr       */
>>>>>>> tderwedu
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
#include <locale>
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

vecServer					ConfigHandler(std::string ConfigFileName);
std::vector<Server const*>	*FindMatchingServers(vecServer const& Servers, size_t Port, std::string IP);
void						printServers(vecServer const& Servers);

#endif
