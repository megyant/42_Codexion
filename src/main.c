/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:38:41 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/27 19:06:25 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_args		*config;
	t_workspace	*workspace;

	config = parsing(ac, av);
	if (!config)
		return (1);
	workspace = init_workspace(config);
	if (!workspace)
		return (ft_free(config), 1);
	start_simulation(workspace);
	free_workspace(workspace);
	free(config);
	return (0);
}
