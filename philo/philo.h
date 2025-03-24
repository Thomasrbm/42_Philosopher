/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 07:27:14 by throbert          #+#    #+#             */
/*   Updated: 2025/03/24 02:04:12 by throbert         ###   ########.fr       */
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
#define RESET_PRECEDENT_STYLE "\e[0;39m"
#define EAT "\e[38;2;5;165;135m\t🍔 is eating \e[0m"
#define THINK "\e[38;2;240;200;50m\t🧠 is thinking  \e[0m"
#define SLEEP "\e[38;2;70;160;240m\t😴 is sleeping  \e[0m"
#define FORK "\e[38;2;190;190;190m\t🍴 has taken a fork\e[0m"
#define DIE "\e[38;2;255;0;0;1m\t❌ died\e[0m"
#define ERROR "One argument is wrong : only positive int and 200 philo max.\n"

typedef struct s_philosopher
{
	int				philo_id_number;
	int				nbmeal_eaten;
	int				left_fork_id;
	int				right_fork_id;
	long long		time_of_last_meal;
	struct s_rules	*rules;
	pthread_t		thread_id;
}					t_philosopher;

typedef struct s_rules
{
	int				nb_of_philo;
	int				time_death;
	int				time_eat;
	int				time_sleep;
	int				nb_to_eat_max;
	int				died;
	int				all_ate;
	long long		first_current_time;
	pthread_mutex_t	last_meal_checker;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_status;
	pthread_mutex_t	death_mutex;
	t_philosopher	*philosophers;
}					t_simulation;

int					main(int argc, char **argv);
int					init_rules(t_simulation *rules, char **argv);

void				print_status(t_simulation *rules, int id, char *string);

void				philo_eats(t_philosopher *philo);
int					launcher(t_simulation *rules);

long				ft_atol(const char *str);
long long			current_time(void);
void				better_usleep(long long time, t_simulation *rules);
void				*philo_routine(void *void_philosopher);
int					is_valid_arg(char **argv);
int					start_simulation(t_simulation *rules);
int					check_death_status(t_simulation *rules);
void				update_meal_info(t_philosopher *philo);
void				death_checker(t_simulation *r, int *i, t_philosopher *p);
