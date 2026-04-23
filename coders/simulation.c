/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 11:46:14 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/23 18:48:58 by mbotelho         ###   ########.fr       */
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
	int i;

	safe_mutex_handle(&dongle->mutex, LOCK, coder->workspace);
	queue_management(coder, dongle);
	while (get_sim_status(coder->workspace) && (heap_peek(dongle) != coder->id
	|| dongle->current_user != -1))
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	if (!get_sim_status(coder->workspace))
	{
		i = -1;
		while (++i < dongle->queue.size)
			if(dongle->queue.heap[i].coder_id == coder->id)
				break;
		remove_heap(dongle, i);
		pthread_cond_broadcast(&dongle->cond);
		safe_mutex_handle(&dongle->mutex, UNLOCK, coder->workspace);
		return (0);
	}
	dongle->current_user = coder->id;
	safe_mutex_handle(&dongle->mutex, UNLOCK, coder->workspace);
	return (1);
}

void	remove_heap(t_dongle *dongle, int i)
{
	if (dongle->queue.size == 0 || i < 0
	|| i >= dongle->queue.size)
		return ;
	dongle->queue.size--;
	if (i < dongle->queue.size)
	{
		dongle->queue.heap[i] = dongle->queue.heap[dongle->queue.size];
		bubble_down(dongle, i);
		bubble_up(dongle, i);
	}
}

void	bubble_down(t_dongle *dongle, int i)
{
	int left;
	int right;
	int smallest;
	t_request temp;
	
	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < dongle->queue.size &&
		(dongle->queue.heap[left].priority_value <
		dongle->queue.heap[smallest].priority_value))
			smallest = left;
		if (right < dongle->queue.size &&
		(dongle->queue.heap[right].priority_value <
		dongle->queue.heap[smallest].priority_value))
			smallest = right;
		if (smallest == i)
			break ;
		temp = dongle->queue.heap[i];
		dongle->queue.heap[i] = dongle->queue.heap[smallest];
		dongle->queue.heap[smallest] = temp;
		i = smallest;
	}
}

int	heap_peek(t_dongle *dongle)
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

void bubble_up(t_dongle *dongle, int i)
{
	int parent_thread;
	t_request temp;

	while (i > 0)
	{
		parent_thread = (i - 1) / 2;
		if (dongle->queue.heap[i].priority_value >=
			dongle->queue.heap[parent_thread].priority_value)
			break ;
		temp = dongle->queue.heap[i];
		dongle->queue.heap[i] = dongle->queue.heap[parent_thread];
		dongle->queue.heap[parent_thread] = temp;
		i = parent_thread;
	}
}

void	insert_heap(t_dongle *dongle, int id, long priority_number)
{
	int i;

	if (dongle->queue.size >= dongle->queue.capacity)
		return ;
	i = dongle->queue.size;
	dongle->queue.heap[i].coder_id = id;
	dongle->queue.heap[i].priority_value = priority_number;
	dongle->queue.size++;
	bubble_up(dongle, i);
}

int	grab_dongles(t_coder *coder)
{
	t_dongle *first;
	t_dongle *second;

	if (coder->left_dongle->dongle_id < coder->right_dongle->dongle_id)
	{
		first = coder->left_dongle;
		second = coder->right_dongle;
	}
	else
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (!request_dongle(coder, first))
		return (0);
	if (!request_dongle(coder, second))
	{
		release_single_dongle(coder, first);
		return (0);
	}
	return (1);
}

void release_single_dongle(t_coder *coder, t_dongle *dongle)
{
	safe_mutex_handle(&dongle->mutex, LOCK, coder->workspace);
	dongle->current_user = -1;
	remove_heap(dongle, 0);
	pthread_cond_broadcast(&dongle->cond);
	safe_mutex_handle(&dongle->mutex, UNLOCK, coder->workspace);
}

void	release_dongles(t_coder *coder)
{
	release_single_dongle(coder, coder->right_dongle);
	release_single_dongle(coder, coder->left_dongle);
}