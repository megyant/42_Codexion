/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 21:02:04 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/30 08:55:26 by mbotelho         ###   ########.fr       */
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

t_args	*init_args(char **av)
{
	t_args	*config;

	config = malloc(sizeof(t_args));
	if (!config)
		return (NULL);
	config->number_coders = ft_atoi(av[1]);
	config->time_burnout = ft_atol(av[2]);
	config->time_compile = ft_atol(av[3]);
	config->time_debug = ft_atol(av[4]);
	config->time_refactor = ft_atol(av[5]);
	config->total_compiles = ft_atoi(av[6]);
	config->dongle_cooldown = ft_atol(av[7]);
	if (strcmp(av[8], "fifo") == 0)
		config->scheduler = 0;
	else
		config->scheduler = 1;
	config = check_final_args(config);
	if (!config)
		return (ft_free(config));
	return (config);
}

t_args	*check_final_args(t_args *config)
{
	if (!config)
		return (NULL);
	if (config->time_compile < 0 || config->time_debug < 0
		|| config->time_refactor < 0 || config->total_compiles < 0
		|| config->dongle_cooldown < 0 || (config->scheduler != 1
			&& config->scheduler != 0) || config->number_coders < 1)
	{
		fprintf(stderr, "Error: Arguments must be positive integers.\n");
		return (ft_free(config));
	}
	if (config->number_coders < 2)
		fprintf(stderr, "Warning: Simulation is doomed whith 1 coder\n");
	if (config->time_compile > config->time_burnout)
	{
		fprintf(stderr,
			"Error: time_to_burnout is too short for a single compile.\n");
		return (ft_free(config));
	}
	if (config->time_burnout < 10)
		fprintf(stderr,
			"Warning: Precision might be lost with burnout times < 10ms.\n");
	if (config->time_burnout <= (config->time_compile + config->time_debug
			+ config->time_refactor))
		fprintf(stderr, "Warning: Burnout time too short to compile\n");
	return (config);
}

int	init_dongles(t_workspace *workspace)
{
	int	i;

	if (!workspace)
		return (1);
	i = -1;
	while (++i < workspace->config->number_coders)
	{
		memset(&workspace->dongles[i], 0, sizeof(t_dongle));
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
