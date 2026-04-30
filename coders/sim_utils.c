/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 11:42:07 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/30 09:02:52 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_message(char *message, t_coder *coder)
{
	safe_mutex_handle(&coder->workspace->print_lock, LOCK, coder->workspace);
	if (!simulation_finished(coder->workspace))
		printf("%ld %d %s\n", elapsed_time(coder->workspace->start_simulation),
			coder->id, message);
	safe_mutex_handle(&coder->workspace->print_lock, UNLOCK, coder->workspace);
}

void	wait_threads(t_workspace *workspace)
{
	while (!get_bool(&workspace->stop_lock, &workspace->all_threads_ready))
		usleep(100);
}
