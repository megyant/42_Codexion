/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 22:21:22 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/30 09:01:17 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*ft_free(void *ptr)
{
	if (!ptr)
		return (NULL);
	free(ptr);
	return (NULL);
}

void	*free_workspace(t_workspace *workspace)
{
	if (!workspace)
		return (NULL);
	free_workspace_dongles(workspace);
	free_workspace_coders(workspace);
	pthread_mutex_destroy(&workspace->stop_lock);
	pthread_mutex_destroy(&workspace->print_lock);
	ft_free(workspace);
	return (NULL);
}

void	*free_workspace_dongles(t_workspace *workspace)
{
	int	i;

	if (workspace->dongles)
	{
		i = -1;
		while (++i < workspace->config->number_coders)
		{
			pthread_mutex_destroy(&workspace->dongles[i].mutex);
			pthread_cond_destroy(&workspace->dongles[i].cond);
		}
		ft_free(workspace->dongles);
	}
	return (NULL);
}

void	*free_workspace_coders(t_workspace *workspace)
{
	int	i;

	if (workspace->coders)
	{
		i = -1;
		while (++i < workspace->config->number_coders)
		{
			safe_mutex_handle(&workspace->coders[i].state_lock, DESTROY,
				workspace);
		}
		ft_free(workspace->coders);
	}
	return (NULL);
}
