/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:38:45 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/04 21:36:24 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_coder	t_coder;

typedef struct s_args
{
	int					number_coders;
	long				time_burnout;
	long				time_compile;
	long				time_debug;
	long				time_refactor;
	int					total_compiles;
	long				dongle_cooldown;
	char				*scheduler;
}						t_args;

typedef struct s_dongle
{
	pthread_mutex_t		dongle;
	int					dongle_id;
}						t_dongle;

typedef struct s_shared
{
	long				start_simulation;
	bool				end_simulation;
	t_dongle			*dongles;
	t_coder				*all_coders;
	t_args				inputs;

}						t_shared;

typedef struct s_coder
{
	int					id;
	long				compile_count;
	bool				finished_compiling;
	long				last_compile_time;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
	pthread_t			thread_id;
	t_shared			*shared_info;
}						t_coder;

// Reading the input
t_args					*parsing(int ac, char **av);
int						check_input(char *arg);
t_args					*allocate_struct(int ac, char **av);

// Error handling
t_args					*input_error(int ac);
void					ft_free(t_args *coders);

// Helper functions
int						is_digit(char c);
long					ft_atol(const char *nptr);
int						ft_atoi(const char *nptr);
char					*ft_strdup(const char *s);

#endif