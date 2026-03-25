/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:38:57 by mbotelho          #+#    #+#             */
/*   Updated: 2026/03/25 19:34:25 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parsing(int ac, char **av)
{
	if (ac != 9)
	{
		fprintf(stderr, "Error: invalid intput\n");
		fprintf(stderr, "\nInput format: number_of_coders time_to_burnout time_to_compile time_to_debug "
			"time_to_refactor number_of_compiles_required dongle_cooldown scheduler(fifo/edf)\n");
	}
	else
		printf("Well done!\n");
}