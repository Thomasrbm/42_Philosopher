/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 23:10:48 by throbert          #+#    #+#             */
/*   Updated: 2025/03/26 00:29:09 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

static void	take_forks(int id, t_simulation *sim)
{
	sem_wait(sim->forks);
	sem_wait(sim->write_sem);
	printf(BOLD "%5ld %3d %s\n" RESET, get_time() - sim->start_time, id + 1,
		FORK);
	sem_post(sim->write_sem);
	sem_wait(sim->forks);
	sem_wait(sim->write_sem);
	printf(BOLD "%5ld %3d %s\n" RESET, get_time() - sim->start_time, id + 1,
		FORK);
	sem_post(sim->write_sem);
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
		take_forks(id, sim);
		check_if_dead(sim, get_time(), sim->last_meal);
		philo_eat(sim, id);
	}
}

// boucle pair et imapir dans utils 2 (2 fonction)

// tester avec 200
