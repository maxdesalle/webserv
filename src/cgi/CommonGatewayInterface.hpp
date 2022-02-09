/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonGatewayInterface.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesalle <mdesalle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 13:07:33 by mdesalle          #+#    #+#             */
/*   Updated: 2022/02/09 15:24:12 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMONGATEWAYINTERFACE_HPP
# define COMMONGATEWAYINTERFACE_HPP

class CommonGatewayInterface
{
	public:

		CommonGatewayInterface(Request &CGIRequest, Server &ServerConfig);
		CommonGatewayInterface(const &ref);
		~CommonGatewayInterface(void);
		CommonGatewayInterface	&operator=(CommonGatewayInterface const &ref);

		char	**GetEnv(void)	const;

	private:

		char	*_env[20] = { "AUTH_TYPE=", "CONTENT_LENGTH=", "CONTENT_TYPE=", "GATEWAY_INTERFACE=CGI/1.1", "PATH_INFO=", "PATH_TRANSLATED=", "QUERY_STRING=", "REMOTE_ADDR=", "REMOTE_HOST=", "REMOTE_IDENT=", "REMOTE_USER=", "REQUEST_METHOD=", "SCRIPT_NAME=", "SERVER_NAME=", "SERVER_PORT=", "SERVER_PROTOCOL=HTTP/1.1", "SERVER_SOFTWARE=WEBSERV/1.0" }

};

#endif
