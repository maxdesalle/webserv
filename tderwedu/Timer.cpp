/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Timer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tderwedu <tderwedu@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 10:22:54 by tderwedu          #+#    #+#             */
/*   Updated: 2022/01/06 12:21:59 by tderwedu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Timer.hpp"

Timer::Timer(void)
{
	clock_gettime(CLOCK_MONOTONIC, &_start);
	_finish = _start;
}

Timer::Timer(const Timer& rhs) : _start(rhs._start), _finish(rhs._finish) {}

Timer::~Timer(void) {}

Timer&		Timer::operator=(const Timer& rhs)
{
	if (this != &rhs)
	{
		_start = rhs._start;
		_finish = rhs._finish;
	}
	return *this;
}

void		Timer::start(void)
{
	clock_gettime(CLOCK_MONOTONIC, &_start);
}

void		Timer::finish(void)
{
	clock_gettime(CLOCK_MONOTONIC, &_finish);
}

double		Timer::elapsedTime(void)
{
	double elapsed;

	elapsed = (_finish.tv_sec - _start.tv_sec) * 1e+3;
	elapsed += (_finish.tv_nsec - _start.tv_nsec) / 1e+6 ;
	return elapsed;
}
