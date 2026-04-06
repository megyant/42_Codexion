/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:38:45 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/06 22:22:10 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_workspace	t_workspace;

typedef struct s_args
{
	int						number_coders;
	long					time_burnout;
	long					time_compile;
	long					time_debug;
	long					time_refactor;
	int						total_compiles;
	long					dongle_cooldown;
	char					*scheduler;
}							t_args;

typedef struct s_request
{
	int						coder_id;
	long					priority_value;
}							t_request;

typedef struct s_priority_queue
{
	t_request				*heap;
	int						size;
	int						capacity;
}							t_priority_queue;

typedef struct s_dongle
{
	int						dongle_id;
	pthread_mutex_t			mutex;
	pthread_cond_t			cond;
	long					last_dongle_usage;
	int						current_user;
	t_priority_queue		queue;
}							t_dongle;

typedef struct s_coder
{
	int						id;
	pthread_t				thread_id;
	long					compile_count;
	long					last_compile_time;
	bool					finished_compiling;
	t_dongle				*left_dongle;
	t_dongle				*right_dongle;
	t_workspace				*workspace;
}							t_coder;

typedef struct s_workspace
{
	t_args					*config;
	t_coder					*coders;
	t_dongle				*dongles;
	long					start_simulation;
	bool					running;
	pthread_mutex_t			stop_lock;
	pthread_mutex_t			print_lock;
}							t_workspace;

// Reading the input
t_args						*parsing(int ac, char **av);
int							check_input(char *arg);
t_args						*allocate_struct(int ac, char **av);

// Error handling
t_args						*input_error(int ac);
t_args						*check_final_args(t_args *config);
void						*free_config(t_args *config);

// Helper functions
int							is_digit(char c);
long						ft_atol(const char *nptr);
int							ft_atoi(const char *nptr);
char						*ft_strdup(const char *s);

#endif