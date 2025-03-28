/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 23:10:48 by throbert          #+#    #+#             */
/*   Updated: 2025/03/28 01:54:50 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	philo_sleep_think(t_simulation *r, int id)
{
	if (sim_stopped(r))
		return ;
	if (check_if_dead(r, get_time(), r->last_meal))
		return ;
	safe_print(get_time() - r->start_time, SLEEP, id + 1, r);
	precise_sleep(r->time_sleep, r);
	if (sim_stopped(r))
		return ;
	safe_print(get_time() - r->start_time, THINK, id + 1, r);
	sem_post(r->any_death_sem);
}

void	philo_eat(t_simulation *r, int id)
{
	sem_wait(r->local_meal_sem);
	r->last_meal = get_time();
	sem_post(r->local_meal_sem);
	safe_print(get_time() - r->start_time, EAT, id + 1, r);
	check_if_dead(r, get_time(), r->last_meal);
	precise_sleep(r->time_eat, r);
	sem_post(r->forks);
	sem_post(r->forks);
	sem_wait(r->meal_sem);
	r->eaten++;
	if (r->nb_eat != -1 && r->eaten >= r->nb_eat)
	{
		sem_wait(r->finished_sem);
		r->finished = 1;
		r->exit_status = 0;
		sem_post(r->finished_sem);
		sem_post(r->meal_sem);
		return ;
	}
	sem_post(r->meal_sem);
}

static void	take_forks(int id, t_simulation *r)
{
	if (sim_stopped(r))
		return ;
	sem_wait(r->forks);
	safe_print(get_time() - r->start_time, FORK, id + 1, r);
	if (sim_stopped(r))
	{
		sem_post(r->forks);
		return ;
	}
	sem_wait(r->forks);
	safe_print(get_time() - r->start_time, FORK, id + 1, r);
}

static void	init_philo_life(int id, t_simulation *r, pthread_t *death_thread)
{
	if (r->nb_philo == 1)
		single_life(id, r);
	init_local_meal_sem(r, id);
	r->philo_id = id;
	sem_wait(r->local_meal_sem);
	r->last_meal = get_time();
	sem_post(r->local_meal_sem);
	pthread_create(death_thread, NULL, monitor_death, r);
}

void	philo_life(int id, t_simulation *r)
{
	pthread_t	death_thread;

	init_philo_life(id, r, &death_thread);
	while (r->finished == 0)
	{
		take_forks(id, r);
		if (sim_stopped(r))
			break ;
		philo_eat(r, id);
		philo_sleep_think(r, id);
		usleep(50);
	}
	pthread_join(death_thread, NULL);
	free(r->pids);
	exit_child(r, r->exit_status);
}
