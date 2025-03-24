/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 23:10:48 by throbert          #+#    #+#             */
/*   Updated: 2025/03/24 06:51:33 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	check_if_dead(t_simulation *sim, long current_time, long last)
{
	sem_wait(sim->finished_sem);
	sem_wait(sim->death_sem);
	if ((current_time - last) >= sim->time_die)
	{
		printf("%5ld %3d %s\n" RESET BOLD, current_time - sim->start_time,
			sim->philo_id + 1, DIE);
		sim->finished = 1;
		kill_remaining(sim, *sim->pids);
		sem_post(sim->death_sem);
		exit(1);
	}
	sem_post(sim->death_sem);
	sem_post(sim->finished_sem);
}

static void	*monitor_death(void *arg)
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
		usleep(100);
	}
	return (NULL);
}

void	philo_sleep_think(t_simulation *sim, int id)
{
	sem_wait(sim->write_sem);
	printf(BOLD "%5ld %3d ", get_time() - sim->start_time, id + 1);
	printf("%s\n" RESET, SLEEP);
	sem_post(sim->write_sem);
	check_if_dead(sim, get_time(), sim->last_meal);
	precise_sleep(sim->time_sleep);
	check_if_dead(sim, get_time(), sim->last_meal);
	sem_wait(sim->write_sem);
	printf(BOLD "%5ld %3d ", get_time() - sim->start_time, id + 1);
	printf("%s\n" RESET, THINK);
	sem_post(sim->write_sem);
}

void	philo_eat(t_simulation *sim, int id)
{
	sem_wait(sim->local_meal_sem);
	sim->last_meal = get_time();
	sem_post(sim->local_meal_sem);
	sem_wait(sim->write_sem);
	printf(BOLD "%5ld %3d ", get_time() - sim->start_time, id + 1);
	printf("%s\n" RESET, EAT);
	sem_post(sim->write_sem);
	check_if_dead(sim, get_time(), sim->last_meal);
	precise_sleep(sim->time_eat);
	sem_post(sim->forks);
	sem_post(sim->forks);
	sem_wait(sim->meal_sem);
	sim->eaten++;
	if (sim->nb_eat != -1 && sim->eaten >= sim->nb_eat)
	{
		sem_wait(sim->finished_sem);
		sim->finished = 1;
		sem_post(sim->finished_sem);
		sem_post(sim->meal_sem);
		exit(0);
	}
	sem_post(sim->meal_sem);
	philo_sleep_think(sim, id);
}

void	philo_life(int id, t_simulation *sim)
{
	pthread_t	death_thread;
	sem_t		local_meal_sem;

	if (sim->nb_philo == 1)
		single_life(id, sim);
	sem_init(&local_meal_sem, 0, 1);
	sim->local_meal_sem = &local_meal_sem;
	sim->philo_id = id;
	sem_wait(sim->local_meal_sem);
	sim->last_meal = get_time();
	sem_post(sim->local_meal_sem);
	pthread_create(&death_thread, NULL, monitor_death, sim);
	pthread_detach(death_thread);
	while (1)
	{
		if (sim->nb_philo % 2 == 1)
		{
			if (id % 2 == 0)
				usleep(500);
			sem_wait(sim->forks);
			sem_wait(sim->write_sem);
			printf(BOLD "%5ld %3d %s\n" RESET, get_time() - sim->start_time, id
				+ 1, FORK);
			sem_post(sim->write_sem);
			sem_wait(sim->forks);
			sem_wait(sim->write_sem);
			printf(BOLD "%5ld %3d %s\n" RESET, get_time() - sim->start_time, id
				+ 1, FORK);
			sem_post(sim->write_sem);
		}
		else
		{
			sem_wait(sim->forks);
			sem_wait(sim->forks);
			sem_wait(sim->write_sem);
			printf(BOLD "%5ld %3d %s\n" RESET, get_time() - sim->start_time, id
				+ 1, FORK);
			printf(BOLD "%5ld %3d %s\n" RESET, get_time() - sim->start_time, id
				+ 1, FORK);
			sem_post(sim->write_sem);
		}
		check_if_dead(sim, get_time(), sim->last_meal);
		philo_eat(sim, id);
	}
}

// boucle pair et imapir dans utils 2 (2 fonction)

// tester avec 200