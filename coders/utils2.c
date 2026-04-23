/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:51:04 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/23 19:22:27 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_message(char *message, t_coder *coder, int id)
{
	long	time;

	safe_mutex_handle(&coder->workspace->print_lock, LOCK, coder->workspace);
	time = get_current_time() - coder->workspace->start_simulation;
	if (coder->workspace->running)
		printf("%ld, %d, %s\n", time, coder->id, message);
	safe_mutex_handle(&coder->workspace->print_lock, UNLOCK, coder->workspace);
}

void	wait_threads(t_workspace *workspace)
{
	while (get_current_time() < workspace->start_simulation)
		ft_usleep(100, workspace);
}
