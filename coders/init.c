/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 21:02:04 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/27 19:48:05 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_workspace	*init_workspace(t_args *config)
{
	t_workspace	*workspace;

	if (!config)
		return (NULL);
	workspace = calloc(1, sizeof(t_workspace));
	if (!workspace)
		return (NULL);
	workspace->config = config;
	workspace->coders = calloc(config->number_coders, sizeof(t_coder));
	workspace->dongles = calloc(config->number_coders, sizeof(t_dongle));
	if (!workspace->coders || !workspace->dongles)
		return (free_workspace(workspace));
	workspace->simulation_finished = false;
	workspace->all_threads_ready = false;
	pthread_mutex_init(&workspace->stop_lock, NULL);
	pthread_mutex_init(&workspace->print_lock, NULL);
	if (init_dongles(workspace) != 0)
		return (free_workspace(workspace));
	if (init_coders(workspace) != 0)
		return (free_workspace(workspace));
	return (workspace);
}

int	init_dongles(t_workspace *workspace)
{
	int	i;

	if (!workspace)
		return (1);
	i = -1;
	while (++i < workspace->config->number_coders)
	{
		memset(&workspace->dongles[i], 0,sizeof(t_dongle));
		workspace->dongles[i].dongle_id = i + 1;
		workspace->dongles[i].current_user = -1;
		workspace->dongles[i].in_use = false;
		workspace->dongles[i].queue_size = 0;
		workspace->dongles[i].seq_counter = 0;
		pthread_mutex_init(&workspace->dongles[i].mutex, NULL);
		pthread_cond_init(&workspace->dongles[i].cond, NULL);
	}
	return (0);
}

int	init_coders(t_workspace *workspace)
{
	int	i;
	int	max;

	i = -1;
	max = workspace->config->number_coders;
	while (++i < max)
	{
		workspace->coders[i].id = i + 1;
		workspace->coders[i].compile_count = 0;
		workspace->coders[i].last_compile_time = workspace->start_simulation;
		workspace->coders[i].finished_compiling = false;
		workspace->coders[i].left_dongle = &workspace->dongles[(i + 1) % max];
		workspace->coders[i].right_dongle = &workspace->dongles[i];
		workspace->coders[i].workspace = workspace;
		workspace->coders[i].last_compile_time = get_current_time();
		safe_mutex_handle(&workspace->coders[i].state_lock, INIT, workspace);
	}
	return (0);
}
