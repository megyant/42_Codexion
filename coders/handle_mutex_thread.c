/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_mutex_thread.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 10:38:25 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/23 19:16:53 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	handle_mutex_error(int status, t_opcode opcode, t_workspace *workspace)
{
	if (0 == status)
		return ;
	if (EINVAL == status && (LOCK == opcode || UNLOCK == opcode))
		fprintf(stderr, "The value specified by mutex is invalid.\n");
	else if (EINVAL == status && INIT == opcode)
		fprintf(stderr, "The value specified by attr is invalid.\n");
	else if (EDEADLK == status)
		fprintf(stderr,
			"A deadlock would occur if the thread blocked"
			"waiting for mutex.\n");
	else if (EPERM == status)
		fprintf(stderr, "The current thread does not hold on mutex.\n");
	else if (ENOMEM == status)
		fprintf(stderr,
			"The process cannot allocate enough memory to create"
			"another mutex.\n");
	else if (EBUSY == status)
		fprintf(stderr, "Mutex is locked.\n");
	if (workspace)
	{
		pthread_mutex_lock(&workspace->stop_lock);
		workspace->running = false;
		pthread_mutex_unlock(&workspace->stop_lock);
	}
}

void	safe_mutex_handle(pthread_mutex_t *mutex, t_opcode opcode,
		t_workspace *workspace)
{
	if (LOCK == opcode)
		handle_mutex_error(pthread_mutex_lock(mutex), opcode, workspace);
	else if (UNLOCK == opcode)
		handle_mutex_error(pthread_mutex_unlock(mutex), opcode, workspace);
	else if (INIT == opcode)
		handle_mutex_error(pthread_mutex_init(mutex, NULL), opcode, workspace);
	else if (DESTROY == opcode)
		handle_mutex_error(pthread_mutex_destroy(mutex), opcode, workspace);
	else
		fprintf(stderr, "Wrong opcode for mutex handle.\n");
}

void	handle_thread_error(int status, t_opcode opcode, t_workspace *workspace)
{
	if (0 == status)
		return ;
	if (EAGAIN == status)
		fprintf(stderr, "No resources to create another thread.\n");
	else if (EPERM == status)
		fprintf(stderr, "The caller does not have appropriate permission.\n");
	else if (EINVAL == status && CREATE == opcode)
		fprintf(stderr, "The value specified by attr is invalid.\n");
	else if (EINVAL == status && (JOIN == opcode || DETACH == opcode))
		fprintf(stderr, "The value specified by thread is not joinable.");
	else if (ESRCH == status)
		fprintf(stderr, "No corresponding thread found.\n");
	else if (EDEADLK == status)
		fprintf(stderr, "Deadlock detected.\n");
	if (workspace)
	{
		pthread_mutex_lock(&workspace->stop_lock);
		workspace->running = false;
		pthread_mutex_unlock(&workspace->stop_lock);
	}
}

void	safe_thread_handle(pthread_t *thread, void *(*routine)(void *),
		void *data, t_opcode opcode)
{
	t_workspace	*workspace;

	workspace = NULL;
	if (data && opcode == CREATE)
		workspace = ((t_coder *)data)->workspace;
	else
		workspace = (t_workspace *)data;
	if (CREATE == opcode)
		handle_thread_error(pthread_create(thread, NULL, routine, data), opcode,
			workspace);
	else if (JOIN == opcode)
		handle_thread_error(pthread_join(*thread, NULL), opcode, workspace);
	else if (DETACH == opcode)
		handle_thread_error(pthread_detach(*thread), opcode, workspace);
	else
		fprintf(stderr, "Wrong opcode for thread handle.\n");
}
