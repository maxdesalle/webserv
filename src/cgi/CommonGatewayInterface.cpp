/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonGatewayInterface.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdesalle <mdesalle@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/09 14:38:27 by mdesalle          #+#    #+#             */
/*   Updated: 2022/02/09 15:23:38 by mdesalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//======================== CONSTRUCTORS / DESTRUCTORS ========================//

CommonGatewayInterface::CommonGatewayInterface(Request &CGIRequest, Server &ServerConfig)
{
}

CommonGatewayInterface::CommonGatewayInterface(CommonGatewayInterface const &ref)
{
	*this = ref;
}

CommonGatewayInterface::~CommonGatewayInterface(void)
{
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


