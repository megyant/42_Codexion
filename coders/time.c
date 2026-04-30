/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gettime.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:35:15 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/27 16:48:02 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		return (fprintf(stderr, "ERROR: gettimeofday() failed.\n"), -1);
	return ((time.tv_sec * 1000L) + (time.tv_usec / 1000L));
}

int	ft_usleep(long miliseconds, t_workspace *workspace)
{
	long	start;

	start = get_current_time();
	while ((get_current_time() - start) < miliseconds)
	{
		if (simulation_finished(workspace))
			break ;
		usleep(500);
	}
	return (0);
}

void	ft_sleep_ms(long ms)
{
	struct timespec	ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1e6;
	nanosleep(&ts, NULL);
}

long	elapsed_time(long start)
{
	return ((long)get_current_time() - start);
}
