/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonGatewayInterface.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 14:38:27 by mdesalle          #+#    #+#             */
/*   Updated: 2022/03/01 16:08:35 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommonGatewayInterface.hpp"

//======================== CONSTRUCTORS / DESTRUCTORS ========================//

CommonGatewayInterface::CommonGatewayInterface(Request &CGIRequest)
{
	const std::map<const std::string, std::string>	CGIVariables = CGIRequest.getCGIServerVars();

	_env[0] = JoinTwoStrings(_env[0], FindValueInMap(CGIVariables, "AUTH_TYPE"));
	_env[1] = JoinTwoStrings(_env[1], FindValueInMap(CGIVariables, "CONTENT_LENGTH"));
	_env[2] = JoinTwoStrings(_env[2], FindValueInMap(CGIVariables, "CONTENT_TYPE"));
	_env[3] = JoinTwoStrings("GATEWAY_INTERFACE=", "CGI/1.1");
	_env[4] = JoinTwoStrings(_env[4], FindValueInMap(CGIVariables, "PATH_INFO"));
	_env[5] = JoinTwoStrings(_env[5], FindValueInMap(CGIVariables, "PATH_TRANSLATED"));
	_env[6] = JoinTwoStrings(_env[6], FindValueInMap(CGIVariables, "QUERY_STRING"));
	_env[7] = JoinTwoStrings(_env[7], FindValueInMap(CGIVariables, "REMOTE_ADDR"));
	_env[8] = JoinTwoStrings(_env[8], FindValueInMap(CGIVariables, "REMOTE_HOST"));
	_env[9] = JoinTwoStrings(_env[9], FindValueInMap(CGIVariables, "REMOTE_IDENT"));
	_env[10] = JoinTwoStrings(_env[10], FindValueInMap(CGIVariables, "REMOTE_USER"));
	_env[11] = JoinTwoStrings(_env[11], FindValueInMap(CGIVariables, "REQUEST_METHOD"));
	_env[12] = JoinTwoStrings(_env[12], FindValueInMap(CGIVariables, "SCRIPT_NAME"));
	_env[13] = JoinTwoStrings(_env[13], FindValueInMap(CGIVariables, "SERVER_NAME"));
	_env[14] = JoinTwoStrings(_env[14], FindValueInMap(CGIVariables, "SERVER_PORT"));
	_env[15] = JoinTwoStrings("SERVER_PROTOCOL=", "HTTP/1.1");
	_env[16] = JoinTwoStrings("SERVER_SOFTWARE=", "WEBSERV/1.0");
}

CommonGatewayInterface::CommonGatewayInterface(const CommonGatewayInterface &ref)
{
	*this = ref;
}

CommonGatewayInterface::~CommonGatewayInterface(void)
{
}

//================================ OPERATORS =================================//

CommonGatewayInterface	&CommonGatewayInterface::operator=(CommonGatewayInterface const &ref)
{
	this->_env = ref.GetEnv();
	return (*this);
}

//============================ GETTERS / SETTERS =============================//

char					**CommonGatewayInterface::GetEnv(void)	const
{
	return (_env);
}

//================================ FUNCTIONS =================================//

char const				*CommonGatewayInterface::FindValueInMap(const std::map<const std::string, std::string> CGIVariables, std::string Key)	const
{
	std::string			Value;

	try
	{
		Value = CGIVariables.at(Key);
	}
	catch (const std::out_of_range&)
	{
		return (NULL);
	}

	return Value.c_str();
}

unsigned int			CommonGatewayInterface::ExecuteCGIScript(void)
{
	FILE				*In = tmpfile();
	FILE				*Out = tmpfile();

	long				FDin = fileno(In);
	long				FDout = fileno(Out);

	size_t				Pid = 0;
	size_t				PrevSTDIN;
	size_t				PrevSTDOUT;

	PrevSTDIN = dup(STDIN_FILENO);
	PrevSTDOUT = dup(STDOUT_FILENO);

	Pid = fork();

	if (Pid < 0)
		return (500); // Internal Server Error
	else if (Pid == 0)
	{
		dup2(FDin, STDIN_FILENO);
		dup2(FDout, STDOUT_FILENO);

		if (execve(GetEnv()[12], NULL, GetEnv()) == -1)
			return LUCAS_ERROR;

		close(FDin);
		close(FDout);

		dup2(PrevSTDIN, 0);
		dup2(PrevSTDOUT, 1);
	}
	else if (Pid > 0)
		waitpid(-1, NULL, 0);
	return (200); // OK
}

size_t					CommonGatewayInterface::StringLength(char const *String)
{
	size_t				Length = 0;

	if (!String)
		return (0);
	while (String[Length])
		Length += 1;
	return (Length);
}

char					*CommonGatewayInterface::JoinTwoStrings(char const *s1, char const *s2)
{
	size_t	i = StringLength(s1);
	size_t	j = StringLength(s2);
	char	*newstring = new char[i + j + 1];

	if (!s1 || !s2)
		return (NULL);
	newstring[i + j] = '\0';
	while (j-- > 0)
		newstring[i + j] = s2[j];
	while (i-- > 0)
		newstring[i] = s1[i];
	return (newstring);
}
