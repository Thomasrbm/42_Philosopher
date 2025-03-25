/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 23:24:00 by throbert          #+#    #+#             */
/*   Updated: 2025/03/25 23:30:56 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	single_life(int id, t_simulation *sim)
{
	sem_wait(sim->write_sem);
	printf(BOLD "%5ld %3d %s\n" RESET, get_time() - sim->start_time, id + 1,
		FORK);
	sem_post(sim->write_sem);
	precise_sleep(sim->time_die);
	sem_wait(sim->write_sem);
	printf(BOLD "%5ld %3d %s\n" RESET, get_time() - sim->start_time, id + 1,
		DIE);
	sem_post(sim->write_sem);
	exit(0);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	check_if_dead(t_simulation *sim, long current_time, long last)
{
	sem_wait(sim->finished_sem);
	sem_wait(sim->death_sem);
	if ((current_time - last) >= sim->time_die)
	{
		sem_wait(sim->write_sem);
		printf(BOLD "%5ld %3d %s\n" RESET, current_time - sim->start_time,
			sim->philo_id + 1, DIE);
		sem_post(sim->write_sem);
		sim->finished = 1;
		kill_remaining(sim, *sim->pids);
		sem_post(sim->death_sem);
		exit(1);
	}
	sem_post(sim->death_sem);
	sem_post(sim->finished_sem);
}

void	*monitor_death(void *arg)
{
	t_simulation	*sim;
	long			current_time;
	long			last_meal;

	sim = (t_simulation *)arg;
	while (1)
	{
		current_time = get_time();
		sem_wait(sim->local_meal_sem);
		last_meal = sim->last_meal;
		sem_post(sim->local_meal_sem);
		check_if_dead(sim, current_time, last_meal);
		usleep(500);
	}
	return (NULL);
}
