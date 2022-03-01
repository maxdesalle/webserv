/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonGatewayInterface.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldelmas <ldelmas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 13:07:33 by mdesalle          #+#    #+#             */
/*   Updated: 2022/03/01 16:03:32 by ldelmas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMONGATEWAYINTERFACE_HPP
# define COMMONGATEWAYINTERFACE_HPP

#include <unistd.h>
#include "../HTTP_Config/Request.hpp"

class Request; //to avoid circular call (but we should correctly set it later)

# define LUCAS_ERROR 10000000; //there was a return I don't know what it had to be

class CommonGatewayInterface
{
	public:

		CommonGatewayInterface(Request &CGIRequest);
		CommonGatewayInterface(const CommonGatewayInterface &ref);
		~CommonGatewayInterface(void);
		CommonGatewayInterface	&operator=(CommonGatewayInterface const &ref);

		char			**GetEnv(void)	const;
		char			*JoinTwoStrings(char const *s1, char const *s2);
		char const		*FindValueInMap(const std::map<const std::string, std::string> CGIVariables, std::string Key)	const;
		size_t			StringLength(char const *String);
		unsigned int	ExecuteCGIScript(void);

	private:

		// char	*_env[20];
		char	**_env; //if you want to make a GetEnv()
};

#endif
