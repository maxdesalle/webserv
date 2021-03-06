/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:30:54 by tderwedu          #+#    #+#             */
/*   Updated: 2022/03/14 17:11:33 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <sys/stat.h>
# include <dirent.h>

# include <string>
# include <sstream>
# include <algorithm>
# include <deque>

bool			ci_equal(const std::string &s1, const std::string &s2);
std::string		getIMFfixdate(const time_t *timestamp);
bool			is_file(const std::string &path);
bool			is_dir(const std::string &path);
std::string		get_autoindex(std::string const& path, std::string const& target);

#endif
