/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:38:57 by mbotelho          #+#    #+#             */
/*   Updated: 2026/03/25 20:10:13 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parsing(int ac, char **av)
{
	int	i;

	i = 0;
	if (ac != 9)
	{
		input_error();
		return 0;
	}
	else
	{
		while (*av && i < (ac - 1))
		{
			if (av[i]++)
			{
				// check errors
				// add to struct
				return 1;
			}
			return 1;
		}
		return 1;
	}
}

void input_error(void)
{
	fprintf(stderr, "Error: invalid input\n");
	fprintf(stderr, "\nInput format: number_of_coders time_to_burnout time_to_compile time_to_debug "
			"time_to_refactor number_of_compiles_required dongle_cooldown scheduler(fifo/edf)\n");
}

int negative_numbers()
{
	return 0;
}
