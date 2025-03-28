/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 07:27:14 by throbert          #+#    #+#             */
/*   Updated: 2025/03/27 00:59:29 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define BOLD "\e[1m"
#define RESET "\e[0m"
#define EAT "\e[38;2;5;165;135m\t🍔 is eating "
#define THINK "\e[38;2;240;200;50m\t🧠 is thinking  "
#define SLEEP "\e[38;2;70;160;240m\t💤 is sleeping  "
#define FORK "\e[38;2;190;190;190m\t🍴 has taken a fork"
#define DIE "\e[38;2;255;0;0;1m\t❌ died\n"

struct	s_rules;
struct	s_philosopher;

typedef struct s_philosopher
{
	int				philo_id;
	int				nb_eaten;
	int				left_fork_id;
	int				right_fork_id;
	long long		t_since_last_meal;
	struct s_rules	*rules;
	pthread_t		thread_id;
}					t_philosopher;

typedef struct s_rules
{
	long			nb_philo;
	long			time_death;
	long			time_eat;
	long			time_sleep;
	long			nb_eat;
	int				died;
	int				all_ate;
	long long		first_current_time;
	pthread_mutex_t	meal_check;
	pthread_mutex_t	forks[200];
	pthread_mutex_t	printing;
	pthread_mutex_t	died_mutex;
	t_philosopher	philosophers[200];
}					t_simulation;

int					main(int argc, char **argv);
int					init_simulation(t_simulation *rules, char **argv);

void				print_status(t_simulation *rules, int id, char *string);

void				philo_eats(t_philosopher *philo);

long				ft_atol(const char *str);
long long			current_time(void);
void				better_sleep(long long time, t_simulation *rules);
void				*philo_routine(void *void_philosopher);
int					ft_isdigit(char c);
int					is_valid_arg(char **argv);
int					launch_philosophers(t_simulation *rules, int i);
void				death__meal_checker(t_simulation *r, int *i,
						t_philosopher *p);
void				*monitor_death(void *arg);
void				single_philo(t_simulation *r, t_philosopher *p);
