/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:38:57 by mbotelho          #+#    #+#             */
/*   Updated: 2026/03/26 19:43:58 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parsing(int ac, char **av)
{
	int	i;

	i = 1;
	if (ac != 9)
		return (input_error(ac));
	while (i < (ac - 1))
	{
		if (!check_input_int(av[i]))
		{
			printf("Error: Negative number as argument\n");
			return (0);
		}
		i++;
	}
	if (strcmp(av[i], "fifo") && strcmp(av[i], "edf"))
	{
		printf("Error: Scheduler must be fifo or edf\n");
		return (0);
	}
	allocate_struct(ac, av);
	return (1);
}

int	input_error(int ac)
{
	if (ac > 9)
		fprintf(stderr, "Error: invalid input. Too many arguments\n");
	else if (ac < 9)
		fprintf(stderr, "Error: invalid input. Too few arguments\n");
	else
		fprintf(stderr, "Error: invalid input.\n");
	fprintf(stderr,
		"\nInput format: number_of_coders time_to_burnout "
		"time_to_compile time_to_debug "
		"time_to_refactor number_of_compiles_required "
		"dongle_cooldown scheduler(fifo/edf)\n");
	return (0);
}

static int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

int	check_input_int(char *arg)
{
	char	*str;

	str = arg;
	if (*arg == '-')
		return (0);
	while (*arg != '\0')
	{
		if (!is_digit(*arg))
			return (0);
		arg++;
	}
	return (1);
}

t_args	allocate_struct(int ac, char **av)
{
	t_args	coders;
}
