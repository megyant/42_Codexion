/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 11:46:14 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/30 09:11:46 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(t_coder *coder)
{
	while (!simulation_finished(coder->workspace))
	{
		if (get_bool(&coder->state_lock, &coder->finished_compiling))
			break ;
		grab_dongles(coder);
		compile(coder);
		release_dongles(coder);
		debug(coder);
		refactor(coder);
	}
	release_dongles(coder);
	return (NULL);
}

void	grab_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	assign_dongles(coder, &first, &second);
	request_dongle(coder, first);
	print_message("has taken a dongle", coder);
	request_dongle(coder, second);
	print_message("has taken a dongle", coder);
	wait_cooldown(coder);
}

void	compile(t_coder *coder)
{
	wait_cooldown(coder);
	safe_mutex_handle(&coder->state_lock, LOCK, coder->workspace);
	coder->last_compile_time = get_current_time();
	coder->compile_count++;
	if (coder->workspace->config->total_compiles != -1
		&& coder->compile_count >= coder->workspace->config->total_compiles)
		coder->finished_compiling = true;
	safe_mutex_handle(&coder->state_lock, UNLOCK, coder->workspace);
	print_message("is compiling", coder);
	ft_usleep(coder->workspace->config->time_compile, coder->workspace);
}

void	debug(t_coder *coder)
{
	print_message("is debugging", coder);
	ft_usleep(coder->workspace->config->time_debug, coder->workspace);
}

void	refactor(t_coder *coder)
{
	print_message("is refactoring", coder);
	ft_usleep(coder->workspace->config->time_refactor, coder->workspace);
}
