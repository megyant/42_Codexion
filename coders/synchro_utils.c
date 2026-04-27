/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchro_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 11:42:07 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/27 11:44:09 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wait_threads(t_workspace *workspace)
{
	while (!get_bool(&workspace->stop_lock, &workspace->all_threads_ready))
		;
}