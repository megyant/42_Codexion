/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gettime.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:35:15 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/23 19:13:09 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
	{
		fprintf(stderr, "ERROR: gettimeofday() failed.\n");
		return (-1);
	}
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	ft_usleep(long miliseconds, t_workspace *workspace)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < miliseconds)
	{
		safe_mutex_handle(&workspace->stop_lock, LOCK, workspace);
		if (!workspace->running)
		{
			safe_mutex_handle(&workspace->stop_lock, UNLOCK, workspace);
			break ;
		}
		safe_mutex_handle(&workspace->stop_lock, UNLOCK, workspace);
		usleep(500);
	}
	return (0);
}
