/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 07:27:14 by throbert          #+#    #+#             */
/*   Updated: 2025/04/01 04:34:21 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

#define BOLD "\e[1m"
#define RESET "\e[0;39m"
#define EAT "\e[0;39m \e[38;2;5;165;135m\t🍔 is eating \e[0m"
#define THINK "\e[0;39m \e[38;2;240;200;50m\t🧠 is thinking  \e[0m"
#define SLEEP "\e[0;39m \e[38;2;70;160;240m\t😴 is sleeping  \e[0m"
#define FORK "\e[0;39m \e[38;2;190;190;190m\t🍴 has taken a fork\e[0m"
#define DIE "\e[38;2;255;0;0;1m\t❌ died \e[0m"
#define ERROR "One argument is wrong : only positive int and 200 philo max.\n"

typedef struct s_simulation
{
	int		nb_philo;
	long	time_die;
	long	time_eat;
	long	time_sleep;
	int		nb_to_eat;
	int		eaten;
	sem_t	*forks;
	sem_t	*write_sem;
	sem_t	*diverse_updt;
	sem_t	*dead;
	pid_t	*pids;
	long	start_time;
	long	last_meal;
	int		philo_id;
	int		exit_status;
}			t_simulation;

int			init_rules(t_simulation *r, char **argv);
long		ft_atol(const char *str);
long		get_time(void);
int			is_valid_arg(char **argv);
void		philo_life(int id, t_simulation *r);
void		wait_philos(t_simulation *r);
void		better_sleep(long milliseconds, t_simulation *r);
void		single_life(int id, t_simulation *r);
void		kill_remaining(t_simulation *r, pid_t pid);
int			check_if_dead(t_simulation *r, long current_time, long last);
void		*monitor_death(void *arg);
void		exit_child(t_simulation *r, int status);
long		get_time(void);
void		safe_print(long timestamp, char *msg, int id, t_simulation *r);
void		safe_sleep_print(t_simulation *r, int id, char *msg, long duration);
void		safe_print_dead(long timestamp, char *msg, int id, t_simulation *r);
void		init_philo_life(int id, t_simulation *r, pthread_t *death_thread);
