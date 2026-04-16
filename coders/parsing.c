/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:38:57 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/16 11:08:25 by mbotelho         ###   ########.fr       */
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
			// check if all but last are numbers and not negative
		{
			printf("Error: Invalid input\n");
			return (NULL);
		}
		i++;
	}
	if (strcmp(av[i], "fifo") && strcmp(av[i], "edf"))
	{
		printf("Error: Scheduler must be fifo or edf\n");
		return (NULL);
	}
	return (init_args(ac, av));
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
	char	*str;

	str = arg;
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

t_args	*init_args(int ac, char **av)
{
	t_args	*config;

	config = malloc(sizeof(t_args));
	if (!config)
		return (NULL);
	config->number_coders = ft_atoi(av[1]);
	config->time_burnout = ft_atol(av[2]);
	config->time_compile = ft_atol(av[3]);
	config->time_debug = ft_atol(av[4]);
	config->time_refactor = ft_atol(av[5]);
	config->total_compiles = ft_atoi(av[6]);
	config->dongle_cooldown = ft_atol(av[7]);
	if (strcmp(av[8], "fifo") == 0)
		config->scheduler = 1; // edf
	else
		config->scheduler = 0; // fifo
	config = check_final_args(config);
	if (!config)
		return (ft_free(config));
	return (config);
}

t_args	*check_final_args(t_args *config)
{
	if (!config)
		return (NULL);
	if (config->time_compile <= 0 || config->time_debug <= 0
		|| config->time_refactor <= 0 || config->total_compiles < 0
		|| config->dongle_cooldown < 0 || (config->scheduler != 1
			&& config->scheduler != 0)) // overall check again for redundancy
	{
		printf("Error: Arguments must be positive integers.\n");
		return (ft_free(config));
	}
	if (config->number_coders < 2) // warning only
		printf("Warning: Only one coder and dongle available. "
			"Simulation is doomed to failure\n");
	if (config->time_compile > config->time_burnout) // error
	{
		printf("Error: time_to_burnout is too short for a single compile.\n");
		return (ft_free(config));
	}
	if (config->time_burnout < 10) // warning only for time percision
		printf("Warning: Precision might be lost with burnout times < 10ms.\n");
	if (config->time_burnout <= (config->time_compile + config->time_debug
			+ config->time_refactor)) // warning only
		printf("Warning: Burnout time is smaller or equal to a coder cycle. "
			"This may lead to burnout\n");
	return (config);
}
