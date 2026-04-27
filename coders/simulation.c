/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 11:46:14 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/27 19:44:04 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile(t_coder *coder)
{
	wait_cooldown(coder);
	safe_mutex_handle(&coder->state_lock, LOCK, coder->workspace);
	coder->last_compile_time = get_current_time();
	coder->compile_count++;
	if (coder->workspace->config->total_compiles != -1 &&
        coder->compile_count >= coder->workspace->config->total_compiles)
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

t_request	remove_heap(t_request *heap, int *size)
{
	t_request	top;

	top = heap[0];
	(*size)--;
	if (*size == 1)
		heap[0] = heap[1];
	return (top);
}

t_request	heap_peek(t_request *heap, int size)
{
	t_request	empty;

	if (size == 0)
	{
		empty.coder_id = -1;
		empty.priority_value = 0;
		empty.ticket = 0;
		return (empty);
	}
	return (heap[0]);
}

void	release_single_dongle(t_coder *coder, t_dongle *dongle)
{
	safe_mutex_handle(&dongle->mutex, LOCK, coder->workspace);
	dongle->last_dongle_usage = get_current_time();
	dongle->in_use = false;
	dongle->current_user = -1;
	pthread_cond_broadcast(&dongle->cond);
	safe_mutex_handle(&dongle->mutex, UNLOCK, coder->workspace);
}

void	release_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	
	assign_dongles(coder, &first, &second);
	release_single_dongle(coder, second);
	release_single_dongle(coder, first);
}
