/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesalle <mdesalle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 11:19:44 by mdesalle          #+#    #+#             */
/*   Updated: 2022/01/28 17:34:39 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "../../include/Webserv.hpp"
/* #include "Location.hpp" */
#include "Server.hpp"

#include <fstream>

void	ConfigHandler(char *file_name);

#endif
