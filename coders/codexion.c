/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:38:41 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/08 20:00:54 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_args		*config;
	t_workspace	*workspace;

	config = parsing(ac, av);
	if (!config)
		return (-1);
	workspace = init_workspace(config);
	free_workspace(workspace);
	ft_free(config);
	return (0);
}
