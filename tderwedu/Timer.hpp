/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 10:10:38 by tderwedu          #+#    #+#             */
/*   Updated: 2022/01/05 12:04:36 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TIMER_HPP
# define TIMER_HPP

#include <ctime>

class Timer
{
private:
	timespec	_start;
	timespec	_finish;
public:
	Timer(void);
	Timer(Timer const& rhs);
	~Timer(void);

	Timer&		operator=(Timer const& rhs);

	void		start(void);
	void		finish(void);
	void		reset(void);

	double		elapsedTime(void);
};

#endif
