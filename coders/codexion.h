/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:38:45 by mbotelho          #+#    #+#             */
/*   Updated: 2026/03/25 20:09:59 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <pthread.h>

typedef struct s_args
{
    int     number_coders;
    long    time_burnout;
    long    time_compile;
    long    time_debug;
    long    time_refactor;
    int     total_compiles;
    long    dongle_cooldown;
    char    **scheduler;
}               t_args;

// Reading the input
int	parsing(int ac, char **av);

// Error handling
void input_error();
int negative_numbers();

#endif