/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 11:48:07 by ldelmas           #+#    #+#             */
/*   Updated: 2022/03/04 10:56:25 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <iostream>
# include <fstream>
# include <sstream>
# include <string>
# include <time.h>
# include "Header.hpp"
# include "Request.hpp"
# include "../cgi/CommonGatewayInterface.hpp"
# include "../config/Location.hpp"

class Request;

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

		bool				FindValueInVector(std::vector<std::string> Haystack, std::string Needle);
		std::string			GetServerVersion(void);
		std::string			GetCurrentFormattedTime(void);
		std::string			FindStatusMessage(unsigned int *StatusCode);
		std::string			GetLastModifiedTimeForFile(std::string Path);
		std::string			HandleNormalPostRequest(Request &HTTPRequest, Location &HTTPLocation, unsigned int *StatusCode);
		std::string const	&GetHeaderResponse(Request &HTTPRequest, Location &HTTPLocation);
		std::string const	&GetBadRequestResponse(Request &HTTPRequest, Location &HTTPLocation, unsigned int StatusCode);
		std::string			GetErrorPagePath(Location &HTTPLocation, unsigned int *StatusCode);
		std::string			ReturnError(Request &HTTPRequest, Location &HTTPLocation, unsigned int *StatusCode);
		std::string			HandleGETRequest(Request &HTTPRequest, Location &HTTPLocation, unsigned int *StatusCode, unsigned int i);
		std::string			HandlePOSTRequest(Request &HTTPRequest, Location &HTTPLocation, unsigned int *StatusCode);
		std::string			HandleCGIPOSTRequest(Request &HTTPRequest, unsigned int *StatusCode);
		std::string			HandleDELETERequest(Request &HTTPRequest, Location &HTTPLocation, unsigned int *StatusCode);

		void				reset(void);
};

#endif
