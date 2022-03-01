/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 11:48:07 by ldelmas           #+#    #+#             */
/*   Updated: 2022/02/28 21:05:55 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include "Header.hpp"

class Response : public Header
{
	private:

		std::string					_HeaderResponse;
		static std::string const	_fieldNames[16];

	public:

		Response(void);
		Response(Response const &src);
		~Response(void);

		Response const &operator=(Response const &right);

		std::string	GetServerVersion(void);
		std::string	FindStatusMessage(int *StatusCode);
		std::string	GetHeaderResponse(int StatusCode);
		std::string	GetCurrentFormattedTime(void);
		std::string	GetLastModifiedTimeForFile(std::string Path);
};

#endif
