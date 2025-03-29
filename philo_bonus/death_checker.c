/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 23:24:00 by throbert          #+#    #+#             */
/*   Updated: 2025/03/28 06:32:11 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	single_life(int id, t_simulation *r)
{
	safe_print(get_time() - r->start_time, FORK, id + 1, r);
	better_sleep(r->time_die, r);
	safe_print_dead(get_time(), DIE, id + 1, r);
	free(r->pids);
	r->exit_status = 1;
	exit_child(r, 1);
}

void	exit_child(t_simulation *r, int status)
{
	sem_close(r->dead);
	sem_close(r->forks);
	sem_close(r->write_sem);
	sem_close(r->diverse_updt);
	exit(status);
}

int	check_if_dead(t_simulation *r, long current_time, long last)
{
	if ((current_time - last) >= r->time_die && r->dead->__align == 1)
	{
		safe_print_dead(current_time, DIE, r->philo_id + 1, r);
		sem_wait(r->diverse_updt);
		r->dead->__align = 0;
		r->exit_status = 1;
		sem_post(r->diverse_updt);
		return (1);
	}
	return (0);
}

static int	check_death(t_simulation *r, long current_time)
{
	sem_wait(r->diverse_updt);
	if (r->dead->__align == 1)
	{
		safe_print_dead(current_time, DIE, r->philo_id + 1, r);
		r->dead->__align = 0;
		r->exit_status = 1;
		sem_post(r->diverse_updt);
		return (1);
	}
	sem_post(r->diverse_updt);
	return (0);
}

void	*monitor_death(void *arg)
{
	t_simulation	*r;
	long			current_time;
	long			last_meal;
	int				exit_status;

	r = (t_simulation *)arg;
	while (1)
	{
		current_time = get_time();
		sem_wait(r->diverse_updt);
		last_meal = r->last_meal;
		exit_status = r->exit_status;
		sem_post(r->diverse_updt);
		if ((current_time - last_meal) >= r->time_die)
		{
			if (check_death(r, current_time))
				break ;
		}
		if (exit_status == 2)
			break ;
		usleep(50);
	}
	return (NULL);
}
