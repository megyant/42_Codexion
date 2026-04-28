/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 10:38:43 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/27 18:38:52 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_bool(pthread_mutex_t *mutex, bool *dest, bool value)
{
	safe_mutex_handle(mutex, LOCK, NULL);
	*dest = value;
	safe_mutex_handle(mutex, UNLOCK, NULL);
}

bool	get_bool(pthread_mutex_t *mutex, bool *value)
{
	bool	pass;

	safe_mutex_handle(mutex, LOCK, NULL);
	pass = *value;
	safe_mutex_handle(mutex, UNLOCK, NULL);
	return (pass);
}

void	set_long(pthread_mutex_t *mutex, long *dest, long value)
{
	safe_mutex_handle(mutex, LOCK, NULL);
	*dest = value;
	safe_mutex_handle(mutex, UNLOCK, NULL);
}

long	get_long(pthread_mutex_t *mutex, long *value)
{
	long	pass;

	safe_mutex_handle(mutex, LOCK, NULL);
	pass = *value;
	safe_mutex_handle(mutex, UNLOCK, NULL);
	return (pass);
}

bool	simulation_finished(t_workspace *workspace)
{
	return (get_bool(&workspace->stop_lock, &workspace->simulation_finished));
}
