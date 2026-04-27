/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_sim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 11:19:24 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/27 20:06:59 by mbotelho         ###   ########.fr       */
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
	return (NULL);
}

void	start_simulation(t_workspace *workspace)
{
	int i;

	if (workspace->config->total_compiles == 0)
		return ;
	i = -1;
	while (++i < workspace->config->number_coders)
		safe_thread_handle(&workspace->coders[i].thread_id,
			codexion_simulator, &workspace->coders[i], CREATE);
	safe_thread_handle(&workspace->monitor_thread, (void *(*)(void *))monitor, workspace, CREATE);
	workspace->start_simulation = get_current_time();
	set_bool(&workspace->stop_lock, &workspace->all_threads_ready, true);
	i = -1;
	while (++i < workspace->config->number_coders)
		safe_thread_handle(&workspace->coders[i].thread_id, NULL, NULL, JOIN);
	safe_thread_handle(&workspace->monitor_thread, NULL, NULL, JOIN);
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

void assign_dongles(t_coder *coder, t_dongle **first, t_dongle **second)
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

void request_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request request;

	safe_mutex_handle(&dongle->mutex, LOCK, coder->workspace);
	request = queue_management(coder, dongle);
	heap_push(dongle, request);
	while (!simulation_finished(coder->workspace))
	{
		if (heap_peek(dongle->queue, dongle->queue_size).coder_id == 
		request.coder_id && !dongle->in_use)
			break;
		safe_mutex_handle(&dongle->mutex, UNLOCK, coder->workspace);
		ft_usleep(1, coder->workspace);
		safe_mutex_handle(&dongle->mutex, LOCK, coder->workspace);
	}
	if (!simulation_finished(coder->workspace))
	{
		dongle->in_use = true;
		dongle->current_user = coder->id;
		remove_heap(dongle->queue, &dongle->queue_size);
	}
	safe_mutex_handle(&dongle->mutex, UNLOCK, coder->workspace);
}

t_request	queue_management(t_coder *coder, t_dongle *dongle)
{
	t_request	request;
	long		last_time;

	request.coder_id = coder->id;
	request.ticket = dongle->seq_counter++;
	if (coder->workspace->config->scheduler == 1)
	{
		last_time = get_long(&coder->state_lock, &coder->last_compile_time);
		request.priority_value = last_time + coder->workspace->config->time_burnout;
	}
	else
		request.priority_value= 0;
	return (request);
}

void heap_push(t_dongle *dongle, t_request request)
{
	t_request	temp;

	if (dongle->queue_size >= 2)
		return ;
	dongle->queue[dongle->queue_size] = request;
	dongle->queue_size++;
	if (dongle->queue_size == 2)
	{
		if (prioritary_queue(dongle->queue[1], dongle->queue[0]))
		{
			temp = dongle->queue[0];
			dongle->queue[0] = dongle->queue[1];
			dongle->queue[1] = temp;
		}
	}
}

int prioritary_queue(t_request new, t_request old)
{
	if (new.priority_value < old.priority_value)
		return (1);
	if (new.priority_value > old.priority_value)
		return (0);
	return (new.ticket < old.ticket);
}

void	wait_cooldown(t_coder *coder)
{
	long	cool_time;
	long	last_used;
	long	right_used;
	long	time_elapsed;
	
	cool_time = coder->workspace->config->dongle_cooldown;
	if (cool_time <= 0)
		return ;
	last_used = get_long(&coder->left_dongle->mutex, 
		&coder->left_dongle->last_dongle_usage);
	right_used = get_long(&coder->right_dongle->mutex, 
		&coder->right_dongle->last_dongle_usage);
	if (right_used > last_used)
		last_used = right_used;
	time_elapsed = get_current_time() - last_used;
	if (time_elapsed < cool_time)
		ft_usleep(cool_time - time_elapsed, coder->workspace);
}

void	*codexion_simulator(void *data)
{
	t_coder	*coder;

	coder = (t_coder *)data;
	wait_threads(coder->workspace);
	if (coder->id % 2 == 0)
		ft_usleep(1, coder->workspace);
	if (coder->left_dongle == coder->right_dongle)
	{
		while(!simulation_finished(coder->workspace))
			ft_usleep(1, coder->workspace);
		return (NULL);
	}
	coder_routine(coder);
	return (NULL);
}