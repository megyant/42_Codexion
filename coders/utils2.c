/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:51:04 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/24 10:59:26 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_message(char *message, t_coder *coder)
{
	long	time;

	safe_mutex_handle(&coder->workspace->print_lock, LOCK, coder->workspace);
	time = get_current_time() - coder->workspace->start_simulation;
	if (get_sim_status(coder->workspace))
		printf("%ld, %d, %s\n", time, coder->id, message);
	safe_mutex_handle(&coder->workspace->print_lock, UNLOCK, coder->workspace);
}

void	wait_threads(t_workspace *workspace)
{
	while (workspace->start_simulation == 0)
		ft_usleep(100, workspace);
}
