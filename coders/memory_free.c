/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 22:21:22 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/06 22:21:51 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*free_config(t_args *config)
{
	if (!config)
		return (NULL);
	if (config->scheduler)
	{
		free(config->scheduler);
		config->scheduler = NULL;
	}
	free(config);
	return (NULL);
}