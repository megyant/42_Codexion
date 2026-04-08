/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:38:45 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/08 20:01:37 by mbotelho         ###   ########.fr       */
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
	int						scheduler;
}							t_args;

typedef struct s_request // The information put in the "Sign-in Sheet"
{
	int						coder_id;
	long					priority_value;
}							t_request;

typedef struct s_priority_queue
{
	t_request				*heap; // The "Sign-in Sheet"
	int						size; // Number of names in the sheet
	int						capacity; // Number of rows in the sheet
}							t_priority_queue;

typedef struct s_dongle
{
	int						dongle_id;
	pthread_mutex_t			mutex; // protect "Sign-in Sheet" for one dongle
	pthread_cond_t			cond; // waiting room
	long					last_dongle_usage;
	int						current_user; // if -1 dongle is free
	t_priority_queue		queue;
}							t_dongle;

typedef struct s_coder
{
	int						id;
	pthread_t				thread_id;
	long					compile_count;
	long					last_compile_time;
	bool					finished_compiling;
	pthread_mutex_t			state_lock; // protect stop lock and last compile time so that we dont have 200 coders checking stop lock at the same time
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
	pthread_mutex_t			stop_lock; // protect simulation status
	pthread_mutex_t			print_lock; // protect terminal output
}							t_workspace;

// Reading the input
t_args						*parsing(int ac, char **av);
int							check_input(char *arg);

// Initialize structures
t_args						*init_args(int ac, char **av);
t_workspace					*init_workspace(t_args *config);


// Error handling
t_args						*input_error(int ac);
t_args						*check_final_args(t_args *config);
void						*ft_free(void *ptr);
void						*free_workspace(t_workspace *workspace);

// Helper functions
int							is_digit(char c);
long						ft_atol(const char *nptr);
int							ft_atoi(const char *nptr);
char						*ft_strdup(const char *s);

#endif