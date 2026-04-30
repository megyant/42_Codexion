/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbotelho <mbotelho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 15:38:45 by mbotelho          #+#    #+#             */
/*   Updated: 2026/04/30 09:34:31 by mbotelho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <errno.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_workspace	t_workspace;

typedef enum e_opcode
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH
}							t_opcode;

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

typedef struct s_request
{
	int						coder_id;
	long					priority_value;
	long					ticket;
}							t_request;

typedef struct s_dongle
{
	int						dongle_id;
	pthread_mutex_t			mutex;
	pthread_cond_t			cond;
	long					last_dongle_usage;
	int						current_user;
	t_request				queue[2];
	int						queue_size;
	int						seq_counter;
	bool					in_use;
}							t_dongle;

typedef struct s_coder
{
	int						id;
	pthread_t				thread_id;
	long					compile_count;
	long					last_compile_time;
	bool					finished_compiling;
	pthread_mutex_t			state_lock;
	t_dongle				*right_dongle;
	t_dongle				*left_dongle;
	t_workspace				*workspace;
}							t_coder;

typedef struct s_workspace
{
	t_args					*config;
	t_coder					*coders;
	t_dongle				*dongles;
	long					start_simulation;
	bool					simulation_finished;
	bool					all_threads_ready;
	pthread_t				monitor_thread;
	pthread_mutex_t			stop_lock;
	pthread_mutex_t			print_lock;
}							t_workspace;

// parsing.c
t_args						*parsing(int ac, char **av);
t_args						*input_error(int ac);
int							check_input(char *arg);

// init.c
t_workspace					*init_workspace(t_args *config);
t_args						*init_args(char **av);
t_args						*check_final_args(t_args *config);
int							init_dongles(t_workspace *workspace);
int							init_coders(t_workspace *workspace);

// sim_utils.c
void						print_message(char *message, t_coder *coder);
void						wait_threads(t_workspace *workspace);
t_args						*extra_errors(t_args *config);

// coder.c
void						*coder_routine(t_coder *coder);
void						grab_dongles(t_coder *coder);
void						compile(t_coder *coder);
void						debug(t_coder *coder);
void						refactor(t_coder *coder);

// dongles.c
void						assign_dongles(t_coder *coder, t_dongle **first,
								t_dongle **second);
void						request_dongle(t_coder *coder, t_dongle *dongle);
void						request_dongle_util(t_coder *coder,
								t_dongle *dongle, t_request request);
void						release_single_dongle(t_coder *coder,
								t_dongle *dongle);
void						release_dongles(t_coder *coder);

// heap.c
t_request					queue_management(t_coder *coder, t_dongle *dongle,
								long last_compile);
void						heap_push(t_dongle *dongle, t_request request);
int							prioritary_queue(t_request new, t_request old);
t_request					heap_peek(t_request *heap, int size);
t_request					remove_heap(t_request *heap, int *size);

// monitor.c
void						*monitor(t_workspace *workspace);
int							burnout(t_workspace *workspace);
void						stop_simulation(t_workspace *workspace);
int							coders_finished(t_workspace *workspace);

// time.c
long						get_current_time(void);
int							ft_usleep(long miliseconds, t_workspace *workspace);
void						ft_sleep_ms(long ms);
long						elapsed_time(long start);

// mutex_thread_safe.c
void						handle_mutex_error(int status, t_opcode opcode,
								t_workspace *workspace);
void						safe_mutex_handle(pthread_mutex_t *mutex,
								t_opcode opcode, t_workspace *workspace);
void						handle_thread_error(int status, t_opcode opcode,
								t_workspace *workspace);
void						safe_thread_handle(pthread_t *thread,
								void *(*routine)(void *), void *data,
								t_opcode opcode);

// free.c
void						*ft_free(void *ptr);
void						*free_workspace(t_workspace *workspace);
void						*free_workspace_dongles(t_workspace *workspace);
void						*free_workspace_coders(t_workspace *workspace);

// utils.c
int							is_digit(char c);
long						ft_atol(const char *nptr);
int							ft_atoi(const char *nptr);
void						*ft_calloc(size_t nmemb, size_t size);

// get_set.c
void						set_bool(pthread_mutex_t *mutex, bool *dest,
								bool value);
bool						get_bool(pthread_mutex_t *mutex, bool *value);
void						set_long(pthread_mutex_t *mutex, long *dest,
								long value);
long						get_long(pthread_mutex_t *mutex, long *value);
bool						simulation_finished(t_workspace *workspace);

// simulation.c
void						*codexion_simulator(void *data);
void						start_simulation(t_workspace *workspace);
void						wait_cooldown(t_coder *coder);

#endif