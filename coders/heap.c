/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 09:11:00 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/30 09:16:35 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_request	queue_management(t_coder *coder, t_dongle *dongle,
		long last_compile)
{
	t_request	request;

	request.coder_id = coder->id;
	request.ticket = dongle->seq_counter++;
	if (coder->workspace->config->scheduler == 1)
		request.priority_value = last_compile
			+ coder->workspace->config->time_burnout;
	else
		request.priority_value = 0;
	return (request);
}

void	heap_push(t_dongle *dongle, t_request request)
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

int	prioritary_queue(t_request new, t_request old)
{
	if (new.priority_value < old.priority_value)
		return (1);
	if (new.priority_value > old.priority_value)
		return (0);
	return (new.ticket < old.ticket);
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

t_request	remove_heap(t_request *heap, int *size)
{
	t_request	top;

	top = heap[0];
	(*size)--;
	if (*size == 1)
		heap[0] = heap[1];
	return (top);
}
