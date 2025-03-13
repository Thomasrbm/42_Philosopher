/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 07:27:14 by throbert          #+#    #+#             */
/*   Updated: 2025/03/03 00:26:01 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#define BOLD "\e[1m"
#define RESET_PRECEDENT_STYLE "\e[0m"
#define EAT "\e[38;2;5;165;135m\t🍔 is eating "
#define THINK "\e[38;2;240;200;50m\t🧠 is thinking  "
#define SLEEP "\e[38;2;70;160;240m\t😴 is sleeping  "
#define FORK "\e[38;2;190;190;190m\t🍴 has taken a fork"
#define DIE "\e[38;2;255;0;0;1m\t❌ died\n"
#define ERROR "One argument is wrong : only positive int and 200 philo max.\n"

struct	s_rules;
struct	s_philosopher;

typedef struct s_philosopher
{
	int				philo_id_number;
	int				nb_meal_eaten;
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
