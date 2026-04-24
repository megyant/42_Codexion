/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:38:41 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/24 10:53:52 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_args		*config;
	t_workspace	*workspace;
	int			i;
	pthread_t	monitor_thread;

	config = parsing(ac, av);
	if (!config)
		return (-1);
	workspace = init_workspace(config);
	if (!workspace)
		return (ft_free(config), -1);
	safe_thread_handle(&monitor_thread, (void *)monitor, workspace, CREATE);
	workspace->start_simulation = get_current_time();
	safe_mutex_handle(&workspace->stop_lock, LOCK, workspace);
	workspace->running = true;
	safe_mutex_handle(&workspace->stop_lock, UNLOCK, workspace);
	i = -1;
	while (++i < config->number_coders)
		safe_thread_handle(&workspace->coders[i].thread_id, NULL, NULL,
			JOIN);
	safe_thread_handle(&monitor_thread, NULL, NULL, JOIN);
	free_workspace(workspace);
	return (ft_free(config), 0);
}
