/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/26 11:48:07 by ldelmas           #+#    #+#             */
/*   Updated: 2022/03/18 12:30:01 by ldelmas          ###   ########.fr       */
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
# include <vector>
# include <map>
# include "Header.hpp"
# include "Request.hpp"
# include "../cgi/CommonGatewayInterface.hpp"
# include "../config/Location.hpp"
# include "../utils/utils.hpp"

class Request;

class Response : public Header
{
	private:

		std::string					_target;
		std::string					_target_path;
		std::string					_Body;
		std::string					_HeaderResponse;
		static std::string const	_fieldNames[16];

	public:

		Response(void);
		Response(Response const &src);
		~Response(void);

		Response const &operator=(Response const &right);

		std::string const	&GetHeaderResponse(Request &HTTPRequest, Location *HTTPLocation);
		std::string const	&GetBadRequestResponse(Request &HTTPRequest, Location *HTTPLocation, unsigned int StatusCode);

		void				reset(void);

	private:
		bool				FindValueInVector(std::vector<std::string> Haystack, std::string Needle);
		std::string			GetServerVersion(void);
		std::string			GetPath(Location *HTTPLocation, unsigned int i);
		std::string			HandleGETCGIRequest(Request &HTTPRequest, Location *HTTPLocation, unsigned int *StatusCode);
		std::string	const	&HandleRedirection(Request &HTTPRequest, Location *HTTPLocation);
		bool				RedirectionExists(Location *HTTPLocation);
		std::string			GetCurrentFormattedTime(void);
		std::string			FindStatusMessage(unsigned int *StatusCode);
		std::string	const	&Handle301Redirect(void);
		std::string			GetLastModifiedTimeForFile(std::string Path);
		std::string const	&GenerateResponse(std::string &Body, unsigned int *StatusCode);
		std::string			HandleNormalPostRequest(Request &HTTPRequest, Location *HTTPLocation, unsigned int *StatusCode);
		std::string			GetErrorPagePath(Location *HTTPLocation, unsigned int *StatusCode);
		std::string 		ReturnError(Location *HTTPLocation, unsigned int *StatusCode);
		std::string			HandleGETRequest(Request &HTTPRequest, Location *HTTPLocation, unsigned int *StatusCode, unsigned int i);
		std::string			HandleGETRequestFile(Request &HTTPRequest, Location *HTTPLocation, unsigned int *StatusCode);
		std::string			CheckIfFileOrFolder(Request &HTTPRequest, Location *HTTPLocation, unsigned int *StatusCode);
		std::string const	&CheckIfFileOrFolderConst(Request &HTTPRequest, Location *HTTPLocation);
		std::string			HandlePOSTRequest(Request &HTTPRequest, Location *HTTPLocation, unsigned int *StatusCode);
		std::string			HandleCGIPOSTRequest(Request &HTTPRequest, unsigned int *StatusCode);
		std::string			HandleDELETERequest(Location *HTTPLocation, unsigned int *StatusCode);

		void				setTargetPath(Request &HTTPRequest, Location *HTTPLocation);
		std::string const	&getTargetPath(void) const;
};

#endif
