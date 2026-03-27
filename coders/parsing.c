/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:38:57 by mbotelho          #+#    #+#             */
/*   Updated: 2026/03/27 09:13:23 by mbotelho         ###   ########.fr       */
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
		if (!check_input_long(av[i]))
		{
			printf("Error: Negative number as argument\n");
			return (NULL);
		}
		i++;
	}
	if (strcmp(av[i], "fifo") && strcmp(av[i], "edf"))
	{
		printf("Error: Scheduler must be fifo or edf\n");
		return (NULL);
	}
	return (allocate_struct(ac, av));
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
		"\nInput format: number_of_coders time_to_burnout "
		"time_to_compile time_to_debug "
		"time_to_refactor number_of_compiles_required "
		"dongle_cooldown scheduler(fifo/edf)\n");
	return (NULL);
}

int	check_input_long(char *arg)
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

t_args	*allocate_struct(int ac, char **av)
{
	t_args	*coders;

	coders = malloc(sizeof(t_args));
	if (!coders)
		return (NULL);
	coders->number_coders = ft_atol(av[1]);
	coders->time_burnout = ft_atol(av[2]);
	coders->time_compile = ft_atol(av[3]);
	coders->time_debug = ft_atol(av[4]);
	coders->time_refactor = ft_atol(av[5]);
	coders->total_compiles = ft_atol(av[6]);
	coders->dongle_cooldown = ft_atol(av[7]);
	coders->scheduler = ft_strdup(av[8]);
	return (coders);
}
