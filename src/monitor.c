/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 10:03:00 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/27 18:59:47 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor(t_workspace *workspace)
{
	wait_threads(workspace);
	while (!simulation_finished(workspace))
	{
		if (burnout(workspace))
			return (NULL);
		if (coders_finished(workspace))
		{
			safe_mutex_handle(&workspace->print_lock, LOCK, workspace);
			printf("All finished!\n");
			safe_mutex_handle(&workspace->print_lock, UNLOCK, workspace);
			return (stop_simulation(workspace), NULL);
		}
		ft_usleep(1, workspace);
	}
	return (NULL);
}

int	burnout(t_workspace *workspace)
{
	int		i;
	long	now;
	long	last_compile;

	i = -1;
	now = get_current_time();
	while (++i < workspace->config->number_coders)
	{
		last_compile = get_long(&workspace->coders[i].state_lock,
				&workspace->coders[i].last_compile_time);
		if (now - last_compile > workspace->config->time_burnout)
		{
			stop_simulation(workspace);
			safe_mutex_handle(&workspace->print_lock, LOCK, workspace);
			printf("%ld %d burned out\n", (now - workspace->start_simulation),
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
		if (!get_bool(&workspace->coders[i].state_lock,
				&workspace->coders[i].finished_compiling))
			return (0);
	}
	return (1);
}

void	stop_simulation(t_workspace *workspace)
{
	int	i;

	safe_mutex_handle(&workspace->stop_lock, LOCK, workspace);
	workspace->simulation_finished = true;
	safe_mutex_handle(&workspace->stop_lock, UNLOCK, workspace);
	i = -1;
	while (++i < workspace->config->number_coders)
	{
		pthread_mutex_lock(&workspace->dongles[i].mutex);
		pthread_cond_broadcast(&workspace->dongles[i].cond);
		pthread_mutex_unlock(&workspace->dongles[i].mutex);
	}
}
