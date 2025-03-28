/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_life_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 23:10:48 by throbert          #+#    #+#             */
/*   Updated: 2025/03/28 06:32:44 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	philo_sleep_think(t_simulation *r, int id)
{
	check_if_dead(r, get_time(), r->last_meal);
	safe_print(get_time() - r->start_time, SLEEP, id + 1, r);
	better_sleep(r->time_sleep, r);
	safe_print(get_time() - r->start_time, THINK, id + 1, r);
	check_if_dead(r, get_time(), r->last_meal);
}

void	philo_eat(t_simulation *r, int id)
{
	sem_wait(r->diverse_updt);
	r->last_meal = get_time();
	sem_post(r->diverse_updt);
	safe_print(get_time() - r->start_time, EAT, id + 1, r);
	check_if_dead(r, get_time(), r->last_meal);
	better_sleep(r->time_eat, r);
	r->eaten++;
	sem_post(r->forks);
	sem_post(r->forks);
	sem_wait(r->diverse_updt);
	if (r->nb_to_eat != -1 && r->eaten >= r->nb_to_eat)
	{
		r->exit_status = 2;
		sem_post(r->diverse_updt);
		return ;
	}
	sem_post(r->diverse_updt);
}

static void	take_forks(int id, t_simulation *r)
{
	sem_wait(r->forks);
	sem_wait(r->forks);
	safe_print(get_time() - r->start_time, FORK, id + 1, r);
	safe_print(get_time() - r->start_time, FORK, id + 1, r);
}

static void	init_philo_life(int id, t_simulation *r, pthread_t *death_thread)
{
	if (r->nb_philo == 1)
		single_life(id, r);
	r->philo_id = id;
	sem_wait(r->diverse_updt);
	r->last_meal = get_time();
	sem_post(r->diverse_updt);
	pthread_create(death_thread, NULL, monitor_death, r);
}

void	philo_life(int id, t_simulation *r)
{
	pthread_t	death_thread;

	init_philo_life(id, r, &death_thread);
	while (1)
	{
		sem_wait(r->diverse_updt);
		if (r->dead->__align != 1 || r->exit_status == 2)
		{
			sem_post(r->diverse_updt);
			break ;
		}
		sem_post(r->diverse_updt);
		take_forks(id, r);
		philo_eat(r, id);
		philo_sleep_think(r, id);
		usleep(50);
	}
	pthread_join(death_thread, NULL);
	free(r->pids);
	exit_child(r, r->exit_status);
}
