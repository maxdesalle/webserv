/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 14:30:39 by mdesalle          #+#    #+#             */
/*   Updated: 2022/03/16 14:46:54 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

//======================== CONSTRUCTORS / DESTRUCTORS ========================//

Location::Location(std::string LocationContent)
{
	size_t		i = 0;
	std::string	KeyValue;
	
	i = FindPath(LocationContent);
	_ClientMaxBodySize = 0;
	_AutoIndex = false;
	if (LocationContent.find("client_max_body_size") == std::string::npos)
		_ClientMaxBodySize = std::string::npos;
	for (; i < LocationContent.size() - 1; i += 1)
	{
		KeyValue = FindKeyValue(LocationContent, i);
		AddToLocationVector(KeyValue);
	}
}

Location::Location(Location const &ref)
{
	*this = ref;
}

Location::~Location(void)
{
}

//================================ OPERATORS =================================//

Location	&Location::operator=(Location const &ref)
{
	this->_Path = ref.GetPath();
	this->_Root = ref.GetRoot();
	this->_Pass = ref.GetPass();
	this->_Index = ref.GetIndex();
	this->_AutoIndex = ref.GetAutoIndex();
	this->_ErrorPage = ref.GetErrorPage();
	this->_Return = ref.GetReturn();
	this->_LimitExcept = ref.GetLimitExcept();
	this->_ClientMaxBodySize = ref.GetClientMaxBodySize();
	return (*this);
}

//============================ GETTERS / SETTERS =============================//

bool const							&Location::GetAutoIndex(void)			const
{
	return (_AutoIndex);
}

size_t const						&Location::GetClientMaxBodySize(void)	const
{
	return (_ClientMaxBodySize);
}

std::string const					&Location::GetPath(void)					const
{
	return (_Path);
}

std::string	const					&Location::GetRoot(void)					const
{
	return (_Root);
}

std::string const					&Location::GetPass(void)					const
{
	return (_Pass);
}

std::vector<std::string> const		&Location::GetIndex(void)				const
{
	return (_Index);
}

std::vector<std::string> const		&Location::GetLimitExcept(void)			const
{
	return (_LimitExcept);
}

std::map<size_t, std::string> const	&Location::GetErrorPage(void)			const
{
	return (_ErrorPage);
}

std::map<size_t, std::string> const	&Location::GetReturn(void)				const
{
	return (_Return);
}


//================================ FUNCTIONS =================================//

/*
 * Saves the location path (like example.com/get (the "get" is the path here))
 */

size_t		Location::FindPath(std::string LocationContent)
{
	size_t		i = 0;
	std::string	NewPath;

	for (i = 0; i < LocationContent.size(); i += 1)
	{
		if (LocationContent[i] == '{')
			break ;
	}
	NewPath = LocationContent.substr(0, i - 1);
	NewPath.erase(std::remove(NewPath.begin(), NewPath.end(), ' '), NewPath.end());
	if (NewPath.empty())
	{
		std::cerr << "Error found when parsing file" << std::endl;
		exit(EXIT_FAILURE);
	}
	_Path = NewPath;
	return (i + 1);
}

/*
 * Returns a substring with just the key and value
 */

std::string	Location::FindKeyValue(std::string LocationContent, size_t &End)
{
	const size_t	Begin = End;

	for (; End < LocationContent.size(); End += 1)
	{
		if (LocationContent[End] == ';')
			break ;
	}
	return (LocationContent.substr(Begin, End - Begin));
}

/*
 * Compares the given string with special keys which have a different behaviour than "normal" keys
 */

bool		Location::SubStringCompare(std::string KeyValue)
{
	static const std::string	SpecialKeys[] = { "return", "error_page" };

	for (size_t i = 0; i < sizeof(SpecialKeys)/ sizeof(std::string); ++i)
	{
		if (KeyValue == SpecialKeys[i])
			return (true);
	}
	return (false);
}

static void		isMethodHandled(std::string const& method)
{
	if (method == "GET" || method == "POST" || method == "DELETE")
		return ;
	std::cerr << "\e[0mError: Webserv does not handle " << method << " method!" << std::endl;
	exit(EXIT_FAILURE);
}

/*
 * Saves the limit_except value(s)
 */

void		Location::SaveLimitExcept(std::string Value)
{
	size_t	EndOfWord = 0;

	if ((EndOfWord = Value.find_first_of(' ', 0)) == std::string::npos)
	{
		_LimitExcept.push_back(Value);
		isMethodHandled(_LimitExcept.back());
		return ;
	}
	for (size_t i = 0; i < Value.size(); i += 1)
	{
		if ((EndOfWord = Value.find_first_of(' ', i)) != std::string::npos)
		{
			_LimitExcept.push_back(Value.substr(i, EndOfWord - i));
			i = EndOfWord;
		}
		else
		{
			_LimitExcept.push_back(Value.substr(i, Value.size() - i));
			break ;
		}
	}
	isMethodHandled(_LimitExcept.back());
}

/*
 * Saves the index value(s)
 */

void		Location::SaveIndex(std::string Value)
{
	size_t	EndOfWord = 0;

	if ((EndOfWord = Value.find_first_of(' ', 0)) == std::string::npos)
	{
		_Index.push_back(Value);
		return ;
	}
	for (size_t i = 0; i < Value.size(); i += 1)
	{
		if ((EndOfWord = Value.find_first_of(' ', i)) != std::string::npos)
		{
			_Index.push_back(Value.substr(i, EndOfWord - i));
			i = EndOfWord;
		}
		else
		{
			_Index.push_back(Value.substr(i, Value.size() - i));
			break ;
		}
	}
}

/*
 * Saves the a return code like "return 301 http://some-link.com" in a map where 301 is the key, and "http://some-link.com" the value
 */

void		Location::SaveReturn(std::string Value)
{
	size_t		ReturnCode = 0;
	size_t		FirstSpaceInString = 0;
	std::string	ReturnURL;

	if ((FirstSpaceInString = Value.find_first_of(' ', 0)) != std::string::npos)
	{
		ReturnCode = size_t(atoi((Value.substr(0, FirstSpaceInString)).c_str()));
		ReturnURL = Value.substr(FirstSpaceInString + 1, Value.size());
		_Return.insert(make_pair(ReturnCode, ReturnURL));
	}
}

/*
 * Saves the a error page like "error_page 404 http://some-link.com" in a map where 404 is the key, and "http://some-link.com" the value
 */

void		Location::SaveErrorPage(std::string Value)
{
	size_t		ErrorCode = 0;
	size_t		FirstSpaceInString = 0;
	std::string ErrorPageURL;

	if ((FirstSpaceInString = Value.find_first_of(' ', 0)) != std::string::npos)
	{
		ErrorCode = size_t(atoi((Value.substr(0, FirstSpaceInString).c_str())));
		ErrorPageURL = Value.substr(FirstSpaceInString + 1, Value.size());
		_ErrorPage.insert(make_pair(ErrorCode, ErrorPageURL));
	}
}

/*
 * Redirects to the right function handlers
 */

void		Location::Assignator(std::string Key, std::string Value)
{
	if (Key == "index")
		SaveIndex(Value);
	else if (Key == "pass")
		_Pass = Value;
	else if (Key == "autoindex")
		_AutoIndex = (Value == "on") ? true : false;
	else if (Key == "limit_except")
		SaveLimitExcept(Value);
	else if (Key == "client_max_body_size")
		_ClientMaxBodySize = size_t(atoi(Value.c_str()) * 1048576);
	else if (Key == "return")
		SaveReturn(Value);
	else if (Key == "error_page")
		SaveErrorPage(Value);
	else if (Key == "root")
	{
		if (Value[Value.size() - 1] == '/')
			Value.resize(Value.size() - 1);
		_Root = Value;
	}
}

/*
 * Adds data parsed in the config file to the location object
 */

void		Location::AddToLocationVector(std::string KeyValue)
{
	size_t		FirstSpaceInString = 0;
	std::string	Key;
	std::string	Value;

	FirstSpaceInString = KeyValue.find_first_of(' ', 0);
	Key = KeyValue.substr(0, FirstSpaceInString);
	Value = KeyValue.substr(FirstSpaceInString + 1, KeyValue.size());
	Assignator(Key, Value);
}

bool		Location::isCgi(void)	const
{
	if (GetPass().empty() == false)
		return (true);
	return (false);
}

bool		Location::isMethodValid(const std::string &method)	const
{
	for (size_t i = 0; i < GetLimitExcept().size(); i += 1)
	{
		if (method == GetLimitExcept()[i])
			return (true);
	}
	return (false);
}

std::ostream&	operator<<(std::ostream& stream, Location const& loc)
{
	stream << "\t\e[34m Location : \e[0m" << std::endl;
	stream << "\t  - Path      : " << "\e[31m>>\e[0m"<< loc._Path << "\e[31m<<\e[0m"<< std::endl;
	stream << "\t  - Root      : " << "\e[31m>>\e[0m" << loc._Root << "\e[31m<<\e[0m" << std::endl;
	stream << "\t  - AutoIndex : " << (loc._AutoIndex ? "True" : "False") << std::endl;
	return stream;
}
