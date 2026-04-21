/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 11:46:14 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/21 21:55:21 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* void	start_simulation(t_workspace *workspace)
{
	if (workspace->config->number_of_compiles_required == 0)
		return ;
	else if (workspace->config->number_of_compiles_required == 1)
		;//to do
	else
		
} */

void	*coder_routine(void *data)
{
	t_coder	*coder;

	coder = (t_coder *)data;
	wait_threads(coder->workspace);
	while (get_sim_status(coder->workspace))
	{
		if(!grab_dongles(coder))
			break;
		compile(coder);
		release_dongles(coder);
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}

void	compile(t_coder *coder)
{
	safe_mutex_handle(&coder->state_lock, LOCK, coder->workspace);
	coder->last_compile_time = get_current_time();
	safe_mutex_handle(&coder->state_lock, UNLOCK, coder->workspace);
	print_message("is compiling", coder, coder->id);
	ft_usleep(coder->workspace->config->time_compile, coder->workspace);
	safe_mutex_handle(&coder->state_lock, LOCK, coder->workspace);
	coder->compile_count++;
	if(coder->compile_count == coder->workspace->config->total_compiles)
		coder->finished_compiling = true;
	safe_mutex_handle(&coder->state_lock, UNLOCK, coder->workspace);
}

void	debug(t_coder *coder)
{
	print_message("is debugging", coder, coder->id);
	ft_usleep(coder->workspace->config->time_debug, coder->workspace);
}

void	refactor(t_coder *coder)
{
	print_message("is refactoring", coder, coder->id);
	ft_usleep(coder->workspace->config->time_refactor, coder->workspace);
}

int	get_sim_status(t_workspace *workspace)
{
	int status;
	
	safe_mutex_handle(&workspace->stop_lock, LOCK, workspace);
	status = (int)workspace->running;
	safe_mutex_handle(&workspace->stop_lock, UNLOCK, workspace);

	return (status);
}



int	request_dongle(t_coder *coder, t_dongle *dongle)
{
	safe_mutex_handle(&dongle->mutex, LOCK, coder->workspace);
	queue_management(coder, dongle);
	while (get_sim_status(coder->workspace) 
		&& check_top_queue(dongle) != coder->coder_id)
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	if (!get_sim_status(coder->workspace))
	{
		remove_heap(dongle->coder_id); // to do
		safe_mutex_handle(&dongle->mutex, UNLOCK, coder->workspace);
		return (0);
	}
	safe_mutex_handle(&dongle->mutex, UNLOCK, coder->workspace);
	return (1);
}

void	remove_heap(int id)
{
	
}

int	check_top_queue(t_dongle *dongle)
{
	if (dongle->queue.size == 0)
		return (-1);
	return (dongle->queue.heap[0].coder_id);
}

void queue_management(t_coder *coder, t_dongle *dongle)
{
	long priority;

	if (coder->workspace->config->scheduler == 1) //edf
		priority = coder->last_compile_time + coder->workspace->config->time_burnout;
	else //fifo
		priority = get_current_time();
	insert_heap(dongle, coder->id, priority);
}

void	insert_heap(t_dongle *dongle, int id, long priority_number)
{
	int i;
	t_request temp;
	int parent_thread; // think of a tree, parent is halfway between children

	i = dongle->queue.size;
	dongle->queue.heap[i].coder_id = id;
	dongle->queue.heap[i].priotity_value = priority_number;
	dongle->queue.size++;

	while (i > 0)
	{
		parent_thread = (i - 1) / 2;
		if (dongle->queue.heap[i].priotity_value >=
			dongle->queue.heap[parent_thread].priotity_value)
			break ;
		temp = dongle->queue.heap[i];
		dongle->queue.heap[i] = dongle->queue.heap[parent_thread];
		dongle->queue.heap[parent_thread] = temp;
		i = parent_thread;
	}
}

int	grab_dongles(t_coder *coder)
{
	request_dongle(coder, coder->right_dongle);
	request_dongle(coder, coder->left_dongle);
	return 0;
}

void	release_dongles(t_coder *coder)
{
}