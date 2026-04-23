/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:38:41 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/23 19:01:25 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_args		*config;
	t_workspace	*workspace;
	int			i;

	config = parsing(ac, av);
	if (!config)
		return (-1);
	workspace = init_workspace(config);
	if (!workspace)
		return (ft_free(config), -1);
	i = -1;
	while (++i < config->number_coders)
		safe_thread_handle(&workspace->coders[i].thread_id, NULL, workspace,
			JOIN);
	free_workspace(workspace);
	return (ft_free(config), 0);
}
