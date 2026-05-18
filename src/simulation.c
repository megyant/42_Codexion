/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 11:19:24 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/30 09:18:07 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*codexion_simulator(void *data)
{
	t_coder	*coder;

	coder = (t_coder *)data;
	wait_threads(coder->workspace);
	if (coder->id % 2 == 0)
		ft_usleep(1, coder->workspace);
	if (coder->left_dongle == coder->right_dongle)
	{
		while (!simulation_finished(coder->workspace))
			ft_usleep(1, coder->workspace);
		return (NULL);
	}
	coder_routine(coder);
	return (NULL);
}

void	start_simulation(t_workspace *workspace)
{
	int	i;

	if (workspace->config->total_compiles == 0)
		return ;
	i = -1;
	while (++i < workspace->config->number_coders)
		safe_thread_handle(&workspace->coders[i].thread_id, codexion_simulator,
			&workspace->coders[i], CREATE);
	safe_thread_handle(&workspace->monitor_thread, (void *(*)(void *))monitor,
		workspace, CREATE);
	workspace->start_simulation = get_current_time();
	set_bool(&workspace->stop_lock, &workspace->all_threads_ready, true);
	i = -1;
	while (++i < workspace->config->number_coders)
		safe_thread_handle(&workspace->coders[i].thread_id, NULL, NULL, JOIN);
	safe_thread_handle(&workspace->monitor_thread, NULL, NULL, JOIN);
}

void	wait_cooldown(t_coder *coder)
{
	long	cool_time;
	long	last_used;
	long	right_used;
	long	time_elapsed;

	cool_time = coder->workspace->config->dongle_cooldown;
	if (cool_time <= 0)
		return ;
	last_used = get_long(&coder->left_dongle->mutex,
			&coder->left_dongle->last_dongle_usage);
	right_used = get_long(&coder->right_dongle->mutex,
			&coder->right_dongle->last_dongle_usage);
	if (right_used > last_used)
		last_used = right_used;
	time_elapsed = get_current_time() - last_used;
	if (time_elapsed < cool_time)
		ft_usleep(cool_time - time_elapsed, coder->workspace);
}
