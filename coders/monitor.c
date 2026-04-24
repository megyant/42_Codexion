/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 10:03:00 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/24 11:41:15 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor(t_workspace *workspace)
{
	wait_threads(workspace);
	while (get_sim_status(workspace))
	{
		if (burnout(workspace))
			return (NULL);
		if (coders_finished(workspace))
			return (stop_simulation(workspace), NULL);
		ft_usleep(10, workspace);
	}
	return (NULL);
}

int	burnout(t_workspace *workspace)
{
	int		i;
	long	current;

	i = -1;
	while (++i < workspace->config->number_coders)
	{
		current = get_current_time();
		if (current
			- workspace->coders[i].last_compile_time > workspace->config->time_burnout)
		{
			safe_mutex_handle(&workspace->print_lock, LOCK, workspace);
			stop_simulation(workspace);
			printf("%ld, %d, burned out\n", (get_current_time()
				- workspace->start_simulation),
			workspace->coders[i].id);
			safe_mutex_handle(&workspace->print_lock, UNLOCK, workspace);
			return (1);
		}
	}
	return (0);
}

int	coders_finished(t_workspace *workspace)
{
	int	i;

	if (workspace->config->total_compiles == -1)
		return (0);
	i = -1;
	while (++i < workspace->config->number_coders)
	{
		if (!workspace->coders[i].finished_compiling)
			return (0);
	}
	return (1);
}

void	stop_simulation(t_workspace *workspace)
{
	int i;
	
	safe_mutex_handle(&workspace->stop_lock, LOCK, workspace);
	if (!workspace->running)
	{
		safe_mutex_handle(&workspace->stop_lock, UNLOCK, workspace);
		return ;
	}
	workspace->running = false;
	safe_mutex_handle(&workspace->stop_lock, UNLOCK, workspace);
	i = -1;
	while (++i < workspace->config->number_coders)
	{
		safe_mutex_handle(&workspace->dongles[i].mutex, LOCK, workspace);
		pthread_cond_broadcast(&workspace->dongles[i].cond);
		safe_mutex_handle(&workspace->dongles[i].mutex, UNLOCK, workspace);
	}	
}