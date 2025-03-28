/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 23:24:00 by throbert          #+#    #+#             */
/*   Updated: 2025/03/28 01:52:35 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	close_all_sems(t_simulation *r)
{
	sem_close(r->any_death_sem);
	sem_close(r->forks);
	sem_close(r->write_sem);
	sem_close(r->meal_sem);
	sem_close(r->death_sem);
	sem_close(r->finished_sem);
	if (r->nb_philo != 1)
		sem_close(r->local_meal_sem);
}

void	single_life(int id, t_simulation *r)
{
	safe_print(get_time() - r->start_time, FORK, id + 1, r);
	precise_sleep(r->time_die, r);
	safe_death(get_time(), id + 1, r);
	free(r->pids);
	r->exit_status = 0;
	exit_child(r, 0);
}

void	exit_child(t_simulation *r, int status)
{
	close_all_sems(r);
	exit(status);
}

int	check_if_dead(t_simulation *r, long current_time, long last)
{
	if ((current_time - last) >= r->time_die)
	{
		r->time_die = get_time();
		r->id_dead = r->philo_id + 1;
		safe_death(current_time, r->philo_id + 1, r);
		return (1);
	}
	return (0);
}

void	*monitor_death(void *arg)
{
	t_simulation	*r;
	long			current_time;
	long			last_meal;

	r = (t_simulation *)arg;
	while (1)
	{
		if (sim_stopped(r))
			break ;
		current_time = get_time();
		sem_wait(r->local_meal_sem);
		last_meal = r->last_meal;
		sem_post(r->local_meal_sem);
		if (check_if_dead(r, current_time, last_meal))
			break ;
		usleep(50);
	}
	return (NULL);
}
