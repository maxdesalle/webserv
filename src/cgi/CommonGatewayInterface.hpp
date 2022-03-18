/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonGatewayInterface.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 13:07:33 by mdesalle          #+#    #+#             */
/*   Updated: 2022/03/18 12:34:52 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMONGATEWAYINTERFACE_HPP
# define COMMONGATEWAYINTERFACE_HPP

# include <unistd.h>
# include <string>
# include <map>
# include "../HTTP_Config/Request.hpp"

# define BUFFER_SIZE 100

class Request; //to avoid circular call (but we should correctly set it later)

class CommonGatewayInterface
{
	typedef std::map<const std::string, std::string> cgiMap;

	public:

		CommonGatewayInterface(Request &CGIRequest);
		CommonGatewayInterface(const CommonGatewayInterface &ref);
		~CommonGatewayInterface(void);
		
		CommonGatewayInterface	&operator=(CommonGatewayInterface const &ref);

		std::string			FindValueInMap(const std::map<const std::string, std::string> CGIVariables, std::string Key)	const;
		unsigned int		ExecuteCGIScript(std::string &targetPath);

		void				resetBody(void);
		std::string	const	&getBody(void) const;

	private:

		cgiMap		&_envMap;
		std::string	_body;

		char *const	*_makeEnv(void);
		char		**_makeArgv(void);
		std::string	&_makeBody(int *fds);
};

#endif
