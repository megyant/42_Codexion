/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:38:45 by mbotelho          #+#    #+#             */
/*   Updated: 2026/03/26 19:33:59 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_args
{
	int		number_coders;
	long	time_burnout;
	long	time_compile;
	long	time_debug;
	long	time_refactor;
	int		total_compiles;
	long	dongle_cooldown;
	char	**scheduler;
}			t_args;

// Reading the input
int			parsing(int ac, char **av);
int			check_input_int(char *arg);

// Error handling
int			input_error(int ac);

// Helper functions
static int	is_digit(char c);

#endif