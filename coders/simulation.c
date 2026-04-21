/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 11:46:14 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/21 12:05:12 by mbotelho         ###   ########.fr       */
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

int	grab_dongles(t_coder *coder)
{
	return 0;
}

void	release_dongles(t_coder *coder)
{
	printf("yayy");
}