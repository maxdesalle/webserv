/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonGatewayInterface.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 14:38:27 by mdesalle          #+#    #+#             */
/*   Updated: 2022/03/18 12:37:36 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommonGatewayInterface.hpp"

//======================== CONSTRUCTORS / DESTRUCTORS ========================//

CommonGatewayInterface::CommonGatewayInterface(Request &CGIRequest) : _envMap(CGIRequest.getCGIServerVars()) {}

CommonGatewayInterface::CommonGatewayInterface(const CommonGatewayInterface &ref) : _envMap(ref._envMap)
{
	this->_body = ref._body;
}

CommonGatewayInterface::~CommonGatewayInterface(void) {}

//================================ OPERATORS =================================//

/*
	I won't create a deep copy for optimization but Request must be destroyed only
	after the CGI.
*/

CommonGatewayInterface	&CommonGatewayInterface::operator=(CommonGatewayInterface const &ref)
{
	this->_envMap = ref._envMap;
	this->_body = ref._body;
	return (*this);
}

/*
	GETTERS AND SETTERS
*/

std::string const	&CommonGatewayInterface::getBody(void) const
{
	return this->_body;
}

//================================ FUNCTIONS =================================//

void				CommonGatewayInterface::resetBody(void)
{
	this->_body = "";
}

std::string			CommonGatewayInterface::FindValueInMap(const std::map<const std::string, std::string> CGIVariables, std::string Key)	const
{
	try {return CGIVariables.at(Key);}
	catch (const std::out_of_range&) {return (NULL);}
}

unsigned int		CommonGatewayInterface::ExecuteCGIScript(std::string &targetPath)
{
	this->_envMap["SCRIPT_NAME"] = targetPath;
	char		**argv = this->_makeArgv();
	int			fds[2];
	size_t		Pid;

	if (pipe(fds) < 0 || (Pid = fork()) < 0)
	 	return 500;
	else if (!Pid)
	{
		if (dup2(fds[1], STDOUT_FILENO) < 0)
			exit(EXIT_FAILURE);
		close(fds[0]);
		close(fds[1]);
		char *const *env = this->_makeEnv();
		execve(argv[0], argv, env);
		close(STDOUT_FILENO);
		for (size_t i = 0; env[i]; i++)
			delete[] env[i];
		delete[] env;
		delete[] argv[0];
		delete[] argv[1];
		delete[] argv;
		exit(EXIT_FAILURE);
	}

	int status;
	waitpid(0, &status, 0);
	if (status/256 == 1)
		return 404;
	else if (status/256)
		return 500;
	this->_makeBody(fds);
	delete[] argv[0];
	delete[] argv;
	return (200);
}

/*
	PRIVATE METHODS
*/

char *const			*CommonGatewayInterface::_makeEnv(void)
{
	char **env = new char*[this->_envMap.size()+1];
	cgiMap::iterator it = this->_envMap.begin();
	for (size_t i = 0; it != this->_envMap.end(); i++, it++)
	{
		std::string var(it->first + "=" + it->second);
		const char *cstr = var.c_str();
		env[i] = new char[var.length()+1];
		strcpy(env[i], cstr);
	}
	env[this->_envMap.size()] = NULL;
	return env;
}

char				**CommonGatewayInterface::_makeArgv(void)
{
	size_t		scriptLength = this->_envMap["CGI_PATH"].length();
	const char	*cscript = this->_envMap["CGI_PATH"].c_str();
	char		**argv = new char *[3];
	argv[0] = new char[scriptLength+1];
	argv[0] = strcpy(argv[0], cscript);
	scriptLength = this->_envMap["SCRIPT_NAME"].length();
	cscript = this->_envMap["SCRIPT_NAME"].c_str();
	argv[1] = new char[scriptLength+1];
	argv[1] = strcpy(argv[1], cscript);
	argv[2] = NULL;
	return argv;
}

std::string			&CommonGatewayInterface::_makeBody(int *fds)
{	
	close(fds[1]);
	int ret = 1;
	lseek(fds[0], 0, SEEK_SET);
	while (ret)
	{
		char	buffer[BUFFER_SIZE] = {0};
		ret = read(fds[0], buffer, BUFFER_SIZE-1);
		this->_body += buffer;
	}
	close(fds[0]);
	wait(0);
	return this->_body;
}
