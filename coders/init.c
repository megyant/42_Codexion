/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 21:02:04 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/06 22:19:45 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_workspace *init_workspace(t_args  *config)
{
    t_workspace *workspace;

    workspace = malloc(sizeof(t_workspace));
    if(!workspace)
        return (NULL);
    workspace->config = config;
    workspace->running = true;
    workspace->start_simulation = 200; // start time placeholder - create function
    workspace->coders = malloc(sizeof(t_coder) * config->number_coders);
    workspace->dongles = malloc(sizeof(t_dongle) * config->number_coders);
    if (!workspace->coders || workspace->dongles)
        return (NULL);
}