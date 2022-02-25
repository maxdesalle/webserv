/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 15:31:19 by tderwedu          #+#    #+#             */
/*   Updated: 2022/02/25 10:23:33 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

bool	ci_equal(const std::string &s1, const std::string &s2)
{
	if (s1.size() != s2.size())
		return false;
	for (size_t i = 0; i < s1.size(); ++i)
	{
		if ((s1[i] | 0x20) != (s2[i] | 0x20))
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
		return (S_IFREG & s_stat.st_mode);
	return false;
}

/*
* Return true is the pathspecified is a directory
*/
bool			is_dir(const std::string &path)
{
	struct stat s_stat;

	if (!stat(path.c_str(), &s_stat))
		return (S_IFDIR & s_stat.st_mode);
	return false;
}

/*
* Produce the autoindex in HTML
* /!\ path MUST be the URI's TARGET
* /!\ The Working Directory MUST be the Location's ROOT
*/
std::string		get_autoindex(const std::string &path)
{
	char								d_buff[18];
	std::string							autoindex;
	std::string							str_name;
	struct tm							*date;
	struct stat							s_stat;
	struct dirent						*entry; // statically allocate struct
	std::stringstream					buff;
	std::deque<std::string>				files;
	std::deque<std::string>				directories;
	std::deque<std::string>::iterator	it;
	std::deque<std::string>::iterator	ite;

	if (!is_dir(path))
		return std::string();
	autoindex.append("<html>\n<head>\n<title>Index of ");
	autoindex.append(path); // Target
	autoindex.append("</title>\n</head>\n<body bgcolor=\"white\">\n<h1>Index of ");
	autoindex.append(path); // Target
	autoindex.append("</h1>\n<hr><pre>\n");
	DIR*	directory = opendir(path.c_str());
	while ((entry = readdir(directory)))
	{
		str_name = std::string(entry->d_name);
		if (str_name == ".")
			continue ;
		// clear the stringstream
		buff.str(std::string());
		// To get the date of Last Modification and File Type
		stat(str_name.c_str(), &s_stat);
		date = gmtime(&s_stat.st_mtime);
		strftime(d_buff, 18, "%d-%b-%Y %R", date);
		if (S_IFREG & s_stat.st_mode)
		{
			buff << "<a href=\"" << str_name << "\">" << str_name <<"</a>";
			buff.width(68 - str_name.length());
			buff << std::right << d_buff;
			buff.width(20);
			buff << s_stat.st_size;
			buff << "\n";
			files.push_back(buff.str());
		}
		else if (S_IFDIR & s_stat.st_mode)
		{
			buff << "<a href=\"" << str_name << "\">" << str_name << "/</a>";
			if (str_name == "..")
			{
				buff << "\n";
				directories.push_back(buff.str());
				continue ;
			}
			buff.width(67 - str_name.length());
			buff << std::right << d_buff;
			buff.width(20);
			buff << "-";
			buff << "\n";
			directories.push_back(buff.str());
		}
	}
	// Sort and then Append Directories
	std::sort(directories.begin(), directories.end());
	it = directories.begin();
	ite = directories.end();
	for (; it< ite; ++it)
		autoindex.append(*it);
	// Sort and then Append Files
	std::sort(files.begin(), files.end());
	it = files.begin();
	ite = files.end();
	for (; it< ite; ++it)
		autoindex.append(*it);
	autoindex.append("</pre><hr>\n</body>\n</html>\n");
	return autoindex;
}
