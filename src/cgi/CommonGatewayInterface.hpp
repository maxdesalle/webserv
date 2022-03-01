/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonGatewayInterface.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 13:07:33 by mdesalle          #+#    #+#             */
/*   Updated: 2022/03/01 14:22:47 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMONGATEWAYINTERFACE_HPP
# define COMMONGATEWAYINTERFACE_HPP

#include <unistd.h>
#include "../HTTP_Config/Request.hpp"

class CommonGatewayInterface
{
	public:

		CommonGatewayInterface(Request &CGIRequest);
		CommonGatewayInterface(const CommonGatewayInterface &ref);
		~CommonGatewayInterface(void);
		CommonGatewayInterface	&operator=(CommonGatewayInterface const &ref);

		char			**GetEnv(void)	const;
		char			*JoinTwoStrings(char const *s1, char const *s2);
		char			*FindValueInMap(const std::map<const std::string, std::string> CGIVariables, std::string Key)	const;
		size_t			StringLength(char const *String);
		unsigned int	ExecuteCGIScript(void);

	private:

		const char	*_env[20] = { "AUTH_TYPE=", "CONTENT_LENGTH=", "CONTENT_TYPE=", "GATEWAY_INTERFACE=CGI/1.1", "PATH_INFO=", "PATH_TRANSLATED=", "QUERY_STRING=", "REMOTE_ADDR=", "REMOTE_HOST=", "REMOTE_IDENT=", "REMOTE_USER=", "REQUEST_METHOD=", "SCRIPT_NAME=", "SERVER_NAME=", "SERVER_PORT=", "SERVER_PROTOCOL=HTTP/1.1", "SERVER_SOFTWARE=WEBSERV/1.0" };

};

#endif
