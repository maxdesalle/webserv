/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 13:44:34 by ldelmas           #+#    #+#             */
/*   Updated: 2022/03/11 15:26:44 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <unistd.h>

// #include <iostream>

int main(int ac, char **av, char **env)
{
	char	*exec[2];
	exec[1] = NULL;
	for (int i = 0; env[i]; i++)
	{
		// std::cout << env[i] << std::endl;
		if (std::string(env[i]).substr(0, 16) == "PATH_TRANSLATED=")
		{
			std::string sexec = std::string(env[i]).substr(16);
			const char *cexec = sexec.c_str();
			exec[0] = (char *)malloc(sizeof(char) * strlen(cexec)+1);
			exec[0] = strcpy(exec[0], cexec);
			break ;
		}
	}
	return execve(exec[0], exec, env);
}
