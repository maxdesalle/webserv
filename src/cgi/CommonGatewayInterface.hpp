/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonGatewayInterface.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesalle <mdesalle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 13:07:33 by mdesalle          #+#    #+#             */
/*   Updated: 2022/03/01 14:33:14 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMONGATEWAYINTERFACE_HPP
# define COMMONGATEWAYINTERFACE_HPP

#include <unistd.h>
#include "../HTTP_Config/Request.hpp"

class Request;

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

		char	*_env[20];
};

#endif
