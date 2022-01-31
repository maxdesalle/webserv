/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/31 11:30:37 by tderwedu          #+#    #+#             */
/*   Updated: 2022/01/31 11:55:48 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <limits.h>
#include <sys/resource.h>

#include <iostream>

int main(void)
{
	long			openmax;
	struct rlimit	s_rl;

	openmax = sysconf(_SC_OPEN_MAX);
	std::cout << "openmax: " << openmax << std::endl;
	#ifdef OPEN_MAX
	std::cout << "OPEN_MAX: " << openmax << std::endl;
	#endif
	getrlimit(RLIMIT_NOFILE, &s_rl);
	std::cout << "RLIMIT cur: " << s_rl.rlim_cur << std::endl;
	std::cout << "RLIMIT max: " << s_rl.rlim_max << std::endl;
}
