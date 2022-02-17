/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:31:19 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/17 18:53:30 by tderwedu         ###   ########.fr       */
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
	//TODO: to do
	(void)path;
	return std::string();
}

/*
* Return a string with the timestamp as specified by HTTP-date (RFC7231)
*/
std::string		getIMFfixdate(const time_t *timestamp)
{
	char		buff[30];
	struct tm	*date; // statically allocate struct

	date = gmtime(timestamp);
	strftime(buff, 30, "%a, %d %b %Y %T GMT", date);
	return std::string(buff);
}

/*
* Return true is the pathspecified is a file
*/
bool			is_file(const std::string &path)
{
	struct stat s_stat;

	if (!stat(path.c_str(), &s_stat))
	{
		if (S_IFDIR & s_stat.st_mode)
			return true;
	}
	return false;
}

/*
* Return true is the pathspecified is a directory
*/
bool			is_dir(const std::string &path)
{
	struct stat s_stat;

	if (!stat(path.c_str(), &s_stat))
	{
		if (S_IFREG & s_stat.st_mode)
			return true;
	}
	return false;
}

//TODO: remvoe ".." and "." from entries
std::string			get_autoindex(const std::string &path)
{
	// char				d_buff[18];
	std::stringstream	buff;
	// std::string			fullpath;
	std::string			str_name;
	// struct tm			*date;
	// struct stat			s_stat;
	struct dirent		*entry; // statically allocate struct

	// if (!is_dir(path))
	// 	return std::string("aie aie aie");
	buff << "<html>\n";
	buff << "<head><title>Index of " << "/error_page/autoindex/" << "</title></head>\n";
	buff << "<body bgcolor=\"white\">\n";
	buff << "<h1>Index of " << "/error_page/autoindex/" << "</h1>\n";
	buff << "<hr><pre>\n";
	DIR* directory = opendir(path.c_str());
	while ((entry = readdir(directory)))
	{
		str_name = std::string(entry->d_name);
		// fullpath = std::string("~/Documents/19/42cursus/14_Webserver/webserv/website") + std::string("/error_page/autoindex/") + str_name;
		// stat(fullpath.c_str(), &s_stat);
		// date = gmtime(&s_stat.st_mtime);
		// strftime(d_buff, 18, "%d-%a-%Y %R", date);
		// if ((S_IFREG  | S_IFDIR) & s_stat.st_mode)
		// {
		// 	buff.width(68);
			buff << "<a href=\"" << str_name << "\">" << str_name <<"/</a>";
			// buff << std::left << d_buff;
			// buff.width(20);
			// if (S_IFDIR & s_stat.st_mode)
			// 	buff << "-";
			// else 
			// 	buff << s_stat.st_size;
			buff << "\n";
		// }
	}
	buff << "</pre><hr>\n</body>\n</html>\n";
	return buff.str();
}

// std::string			get_autoindex(const std::string &path, RequestHandler &handler)
// {
// 	char				d_buff[18];
// 	std::stringstream	buff;
// 	std::string			fullpath;
// 	std::string			str_name;
// 	struct tm			*date;
// 	struct stat			s_stat;
// 	struct dirent		*entry; // statically allocate struct

// 	if (!is_dir(path))
// 		return ;
// 	buff << "<html>\n";
// 	buff << "<head><title>Index of " << handler.getTarget() << "</title></head>\n";
// 	buff << "<body bgcolor=\"white\">\n";
// 	buff << "<h1>Index of " << handler.getTarget() << "</h1>\n";
// 	buff << "<hr><pre>\n";
// 	DIR* directory = opendir(path.c_str());
// 	while ((entry = readdir(directory)))
// 	{
// 		str_name = std::string(entry->d_name);
// 		fullpath = handler.getRoot() + handler.getTarget() + str_name;
// 		stat(fullpath.c_str(), &s_stat);
// 		date = gmtime(s_stat.st_mtime);
// 		strftime(d_buff, 18, "%d-%a-%Y %R", date);
// 		if ((S_IFREG  | S_IFDIR) & s_stat.st_mode)
// 		{
// 			buff.width(68);
// 			buff << "<a href=\"" << str_name << "\">" << str_name <<"/</a>";
// 			buff << std::left << d_buff;
// 			buff.width(20);
// 			buff << (S_IFDIR & s_stat.st_mode ? "-": s_stat.st_size) << "\n";
// 		}
// 	}
// 	return buff.str();
// }
