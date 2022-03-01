/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonGatewayInterface.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 14:38:27 by mdesalle          #+#    #+#             */
/*   Updated: 2022/03/01 19:03:29 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommonGatewayInterface.hpp"

//======================== CONSTRUCTORS / DESTRUCTORS ========================//

CommonGatewayInterface::CommonGatewayInterface(Request &CGIRequest)
{
	this->_env = new char* [18];
	const std::map<const std::string, std::string>	CGIVariables = CGIRequest.getCGIServerVars();

	_env[0] = JoinTwoStrings("AUTH_TYPE=", FindValueInMap(CGIVariables, "AUTH_TYPE"));
	_env[1] = JoinTwoStrings("CONTENT_LENGTH=", FindValueInMap(CGIVariables, "CONTENT_LENGTH"));
	_env[2] = JoinTwoStrings("CONTENT_TYPE=", FindValueInMap(CGIVariables, "CONTENT_TYPE"));
	_env[3] = JoinTwoStrings("GATEWAY_INTERFACE=", "CGI/1.1");
	_env[4] = JoinTwoStrings("PATH_INFO=", FindValueInMap(CGIVariables, "PATH_INFO"));
	_env[5] = JoinTwoStrings("PATH_TRANSLATED=", FindValueInMap(CGIVariables, "PATH_TRANSLATED"));
	_env[6] = JoinTwoStrings("QUERY_STRING=", FindValueInMap(CGIVariables, "QUERY_STRING"));
	_env[7] = JoinTwoStrings("REMOTE_ADDR=", FindValueInMap(CGIVariables, "REMOTE_ADDR"));
	_env[8] = JoinTwoStrings("REMOTE_HOST=", FindValueInMap(CGIVariables, "REMOTE_HOST"));
	_env[9] = JoinTwoStrings("REMOTE_IDENT=", FindValueInMap(CGIVariables, "REMOTE_IDENT"));
	_env[10] = JoinTwoStrings("REMOTE_USER=", FindValueInMap(CGIVariables, "REMOTE_USER"));
	_env[11] = JoinTwoStrings("REQUEST_METHOD=", FindValueInMap(CGIVariables, "REQUEST_METHOD"));
	_env[12] = JoinTwoStrings("SCRIPT_NAME=", FindValueInMap(CGIVariables, "SCRIPT_NAME"));
	_env[13] = JoinTwoStrings("SERVER_NAME=", FindValueInMap(CGIVariables, "SERVER_NAME"));
	_env[14] = JoinTwoStrings("SEVER_PORT=", FindValueInMap(CGIVariables, "SERVER_PORT"));
	_env[15] = JoinTwoStrings("SERVER_PROTOCOL=", "HTTP/1.1");
	_env[16] = JoinTwoStrings("SERVER_SOFTWARE=", "WEBSERV/1.0");
	_env[17] = NULL;
}

CommonGatewayInterface::CommonGatewayInterface(const CommonGatewayInterface &ref)
{
	*this = ref;
}

CommonGatewayInterface::~CommonGatewayInterface(void)
{
	delete [] this->_env;
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

std::string				CommonGatewayInterface::FindValueInMap(const std::map<const std::string, std::string> CGIVariables, std::string Key)	const
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

	return (Value);
}

unsigned int			CommonGatewayInterface::ExecuteCGIScript(void)
{
	char				*ScriptPath = CreateSubString(GetEnv()[12], 12, StringLength(GetEnv()[12]) - 12);
	char				*argv[2] = {ScriptPath, NULL};
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
	{
		delete ScriptPath;
		return (500); // Internal Server Error
	}
	else if (Pid == 0)
	{
		dup2(FDin, STDIN_FILENO);
		dup2(FDout, STDOUT_FILENO);

		if (execve(ScriptPath, argv, GetEnv()) == -1)
		{
			delete ScriptPath;
			return LUCAS_ERROR;
		}

		close(FDin);
		close(FDout);

		dup2(PrevSTDIN, 0);
		dup2(PrevSTDOUT, 1);
	}
	else if (Pid > 0)
		waitpid(-1, NULL, 0);

	delete ScriptPath;
	return (200); // OK
}

size_t					CommonGatewayInterface::StringLength(char const *s)
{
	size_t				len = 0;

	if (!s)
		return (0);
	while (s[len])
		len += 1;
	return (len);
}

char					*CommonGatewayInterface::CreateSubString(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	unsigned int	j = StringLength(s);
	char			*substring = new char [len + 1];

	i = 0;
	if (j == 0)
		return (NULL);
	if (start < j)
	{
		while (s[start] && len-- > 0)
		{
			substring[i] = s[start];
			start++;
			i++;
		}
	}
	substring[i] = '\0';
	return (substring);
}

char					*CommonGatewayInterface::JoinTwoStrings(std::string s1, std::string s2)
{
	size_t	i = s1.size();
	size_t	j = s2.size();
	char	*newstring = new char[i + j + 1];

	if (s1.empty() || s2.empty())
		return (NULL);
	newstring[i + j] = '\0';
	while (j-- > 0)
		newstring[i + j] = s2[j];
	while (i-- > 0)
		newstring[i] = s1[i];
	return (newstring);
}
