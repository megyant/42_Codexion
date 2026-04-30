/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 09:06:25 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/30 09:32:59 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	assign_dongles(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
	{
		*first = coder->left_dongle;
		*second = coder->right_dongle;
	}
	else
	{
		*first = coder->right_dongle;
		*second = coder->left_dongle;
	}
}

void	request_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request	request;
	long		last_compile;

	last_compile = get_long(&coder->state_lock, &coder->last_compile_time);
	safe_mutex_handle(&dongle->mutex, LOCK, coder->workspace);
	request = queue_management(coder, dongle, last_compile);
	heap_push(dongle, request);
	request_dongle_util(coder, dongle, request);
	if (!simulation_finished(coder->workspace))
	{
		dongle->in_use = true;
		dongle->current_user = coder->id;
		remove_heap(dongle->queue, &dongle->queue_size);
	}
	safe_mutex_handle(&dongle->mutex, UNLOCK, coder->workspace);
}

void	request_dongle_util(t_coder *coder, t_dongle *dongle, t_request request)
{
	while (1)
	{
		if (heap_peek(dongle->queue,
				dongle->queue_size).coder_id == request.coder_id
			&& !dongle->in_use)
			break ;
		safe_mutex_handle(&dongle->mutex, UNLOCK, coder->workspace);
		if (simulation_finished(coder->workspace))
		{
			safe_mutex_handle(&dongle->mutex, LOCK, coder->workspace);
			break ;
		}
		ft_usleep(1, coder->workspace);
		safe_mutex_handle(&dongle->mutex, LOCK, coder->workspace);
	}
}

void	release_single_dongle(t_coder *coder, t_dongle *dongle)
{
	safe_mutex_handle(&dongle->mutex, LOCK, coder->workspace);
	if (dongle->current_user != coder->id)
	{
		safe_mutex_handle(&dongle->mutex, UNLOCK, coder->workspace);
		return ;
	}
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
	release_single_dongle(coder, first);
	release_single_dongle(coder, second);
}
