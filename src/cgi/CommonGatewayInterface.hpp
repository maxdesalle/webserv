/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonGatewayInterface.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 13:07:33 by mdesalle          #+#    #+#             */
/*   Updated: 2022/03/07 21:57:34 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMONGATEWAYINTERFACE_HPP
# define COMMONGATEWAYINTERFACE_HPP

#include <unistd.h>
#include <string>
#include <map>
#include "../HTTP_Config/Request.hpp"

class Request; //to avoid circular call (but we should correctly set it later)

class CommonGatewayInterface
{
	typedef std::map<const std::string, std::string> cgiMap;

	public:

		CommonGatewayInterface(Request &CGIRequest);
		CommonGatewayInterface(const CommonGatewayInterface &ref);
		~CommonGatewayInterface(void);
		
		CommonGatewayInterface	&operator=(CommonGatewayInterface const &ref);

		std::string		FindValueInMap(const std::map<const std::string, std::string> CGIVariables, std::string Key)	const;
		unsigned int	ExecuteCGIScript(void);

		// void				SetEnv(Request &CGIRequest);
		// std::string			*GetEnv(void)	const;

	private:

		cgiMap	&_envMap;

		char *const	*_makeEnv(void);
};

#endif
