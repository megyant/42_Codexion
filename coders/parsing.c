/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:38:57 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/30 09:29:31 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_args	*parsing(int ac, char **av)
{
	int	i;

	i = 1;
	if (ac != 9)
		return (input_error(ac));
	while (i < (ac - 1))
	{
		if (!check_input(av[i]))
		{
			fprintf(stderr, "Error: Invalid input\n");
			return (NULL);
		}
		i++;
	}
	if (strcmp(av[i], "fifo") && strcmp(av[i], "edf"))
	{
		fprintf(stderr, "Error: Scheduler must be fifo or edf\n");
		return (NULL);
	}
	return (init_args(av));
}

t_args	*input_error(int ac)
{
	if (ac > 9)
		fprintf(stderr, "Error: invalid input. Too many arguments\n");
	else if (ac < 9)
		fprintf(stderr, "Error: invalid input. Too few arguments\n");
	else
		fprintf(stderr, "Error: invalid input.\n");
	fprintf(stderr,
		"\nInput format: ./codexion number_of_coders time_to_burnout "
		"time_to_compile time_to_debug "
		"time_to_refactor number_of_compiles_required "
		"dongle_cooldown scheduler(fifo/edf)\n");
	return (NULL);
}

int	check_input(char *arg)
{
	if (*arg == '\0' || *arg == '-')
		return (0);
	while (*arg != '\0')
	{
		if (!is_digit(*arg))
			return (0);
		arg++;
	}
	return (1);
}
