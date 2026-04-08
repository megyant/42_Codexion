/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 21:02:04 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/08 20:02:00 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_workspace	*init_workspace(t_args *config)
{
	t_workspace	*workspace;

    if (!config)
        return (NULL);
	workspace = malloc(sizeof(t_workspace));
	if (!workspace)
		return (NULL);
    workspace->coders = malloc(sizeof(t_coder) * config->number_coders);
	workspace->dongles = malloc(sizeof(t_dongle) * config->number_coders);
    if (!workspace->coders || !workspace->dongles)
    {
        free(workspace->coders);
        free(workspace->dongles);
        free(workspace);
        return (NULL);
    }
	workspace->config = config;
	workspace->running = true;
    pthread_mutex_init(&workspace->stop_lock, NULL);
    pthread_mutex_init(&workspace->stop_lock, NULL);
    workspace->start_simulation = 0; // start only after all background processes are done
    return (workspace);
}



/* int	init_dongles(t_workspace *ws)
{
	int	i;
	int	n;

	n = ws->config->number_coders;
	i = 0;
	while (i < n)
	{
		ws->dongles[i].id = i;
		ws->dongles[i].last_released_time = 0;
		ws->dongles[i].current_owner = -1;
		// At most,
			2 coders (left and right) will ever wait for a specific dongle
		ws->dongles[i].queue.capacity = 2;
		ws->dongles[i].queue.size = 0;
		ws->dongles[i].queue.heap = malloc(sizeof(t_request) * 2);
		if (!ws->dongles[i].queue.heap)
			return (0);
		pthread_mutex_init(&ws->dongles[i].mutex, NULL);
		pthread_cond_init(&ws->dongles[i].cond, NULL);
		i++;
	}
	return (1);
}

// 2. Initialize the Coders (and link their dongles)
void	init_coders(t_workspace *ws)
{
	int	i;
	int	n;

	n = ws->config->number_coders;
	i = 0;
	while (i < n)
	{
		ws->coders[i].id = i + 1; // Coders are 1-indexed according to subject
		ws->coders[i].compile_count = 0;
		ws->coders[i].last_compile_start = ws->start_simulation;
		ws->coders[i].finished = false;
		ws->coders[i].workspace = ws;
		// The Circular Logic
		ws->coders[i].left_dongle = &ws->dongles[i];
		ws->coders[i].right_dongle = &ws->dongles[(i + 1) % n];
		i++;
	}
}
*/ 