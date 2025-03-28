/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 07:27:14 by throbert          #+#    #+#             */
/*   Updated: 2025/03/28 01:39:09 by throbert         ###   ########.fr       */
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
	long 	time_of_death;
	int		id_dead;
	int		nb_philo;
	long	time_die;
	long	time_eat;
	long	time_sleep;
	int		nb_eat;
	int		eaten;
	sem_t	*forks;
	sem_t	*write_sem;
	sem_t	*meal_sem;
	sem_t	*death_sem;
	sem_t	*finished_sem;
	pid_t	*pids;
	sem_t	*local_meal_sem;
	long	start_time;
	long	last_meal;
	int		philo_id;
	int		finished;
	sem_t	*any_death_sem;
	int		exit_status;
}			t_simulation;

int			init_rules(t_simulation *r, char **argv);
long		ft_atol(const char *str);
long		get_time(void);
int			is_valid_arg(char **argv);
void		philo_life(int id, t_simulation *r);
void		wait_philos(t_simulation *r);
void		precise_sleep(long milliseconds, t_simulation *r);
void		single_life(int id, t_simulation *r);
void		kill_remaining(t_simulation *r, pid_t pid);
int			check_if_dead(t_simulation *r, long current_time, long last);
void		*monitor_death(void *arg);
void		exit_child(t_simulation *r, int status);
char		*ft_strjoin(char *s1, char *s2);
char		*ft_itoa(int n);
long		get_time(void);
int			sim_stopped(t_simulation *r);
void		init_local_meal_sem(t_simulation *r, int id);
void	safe_print(long timestamp, char *msg,  int id, t_simulation *r);
void		safe_death(long current_time, int id, t_simulation *r);
void		safe_sleep_print(t_simulation *r, int id, char *msg, long duration);
