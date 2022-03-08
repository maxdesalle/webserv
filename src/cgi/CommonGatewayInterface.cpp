/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonGatewayInterface.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 14:38:27 by mdesalle          #+#    #+#             */
/*   Updated: 2022/03/08 09:37:02 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommonGatewayInterface.hpp"

//======================== CONSTRUCTORS / DESTRUCTORS ========================//

CommonGatewayInterface::CommonGatewayInterface(Request &CGIRequest) : _envMap(CGIRequest.getCGIServerVars()) {}

CommonGatewayInterface::CommonGatewayInterface(const CommonGatewayInterface &ref) : _envMap(ref._envMap) {}

CommonGatewayInterface::~CommonGatewayInterface(void) {}

//================================ OPERATORS =================================//

/*
	I won't create a deep copy for optimization but Request must be destroyed only
	after the CGI.
*/

CommonGatewayInterface	&CommonGatewayInterface::operator=(CommonGatewayInterface const &ref)
{
	this->_envMap = ref._envMap;
	return (*this);
}

//================================ FUNCTIONS =================================//

std::string			CommonGatewayInterface::FindValueInMap(const std::map<const std::string, std::string> CGIVariables, std::string Key)	const
{
	try {return CGIVariables.at(Key);}
	catch (const std::out_of_range&) {return (NULL);}
}


unsigned int		CommonGatewayInterface::ExecuteCGIScript(void)
{
	size_t		scriptLength = this->_envMap["SCRIPT_NAME"].length()-12;
	std::string	tmp = this->_envMap["SCRIPT_NAME"].substr(12, scriptLength);
	const char	*cscript = tmp.c_str();
	char		script[scriptLength];
	strcpy(script, cscript);
	char		*argv[2] = {script, NULL};

	FILE		*In = tmpfile();
	FILE		*Out = tmpfile();

	int			FDin = fileno(In);
	int			FDout = fileno(Out);

	size_t		Pid = 0;

	Pid = fork();

	if (Pid < 0)
		return 500; // Internal Server Error
	else if (Pid == 0)
	{
		dup2(FDin, STDIN_FILENO);
		dup2(FDout, STDOUT_FILENO);
		close(FDin);
		close(FDout);
		if (execve(argv[0], argv, this->_makeEnv()) == -1) //will have to delete env
			return 500;
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
	}
	else if (Pid > 0)
		waitpid(-1, NULL, 0); //should probably change the -1 by a zero because it shouldn't wait for any process to end
	return (200); // OK
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
		env[i] = new char[var.length()];
		strcpy(env[i], cstr);
	}
	env[this->_envMap.size()] = NULL;
	return env;
}
