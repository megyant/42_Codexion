/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 21:02:04 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/24 11:09:41 by mbotelho         ###   ########.fr       */
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
	workspace->running = false;
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
		workspace->dongles[i].dongle_id = i;
		workspace->dongles[i].last_dongle_usage = 0;
		workspace->dongles[i].current_user = -1;
		pthread_mutex_init(&workspace->dongles[i].mutex, NULL);
		pthread_cond_init(&workspace->dongles[i].cond, NULL);
		if (init_queue(&workspace->dongles[i].queue, workspace->config) != 0)
			return (1);
	}
	return (0);
}

int	init_queue(t_priority_queue *queue, t_args *config)
{
	if (!queue || !config)
		return (1);
	queue->heap = malloc(sizeof(t_request) * (config->number_coders + 1));
	if (!queue->heap)
		return (1);
	queue->size = 0;
	queue->capacity = config->number_coders;
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
		workspace->coders[i].last_compile_time = get_current_time();
		workspace->coders[i].finished_compiling = false;
		workspace->coders[i].workspace = workspace;
		workspace->coders[i].left_dongle = &workspace->dongles[i];
		workspace->coders[i].right_dongle = &workspace->dongles[(i + 1) % max];
		safe_mutex_handle(&workspace->coders[i].state_lock, INIT, workspace);
		safe_thread_handle(&workspace->coders[i].thread_id, coder_routine,
			&workspace->coders[i], CREATE);
	}
	return (0);
}

int	init_threads(t_workspace *workspace, int max)
{
	int	i;

	i = -1;
	while (++i < max)
	{
		if (workspace->running == false)
			return (1);
	}
	return (0);
}
