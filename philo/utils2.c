/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 21:45:43 by throbert          #+#    #+#             */
/*   Updated: 2025/03/26 01:32:51 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_if_philo_died(t_simulation *r, int *i, t_philosopher *p)
{
	while (*i < r->nb_philo && !r->died)
	{
		pthread_mutex_lock(&r->meal_check);
		if (current_time() - p[*i].t_since_last_meal > r->time_death)
		{
			print_status(r, *i, DIE);
			pthread_mutex_lock(&r->died_mutex);
			r->died = 1;
			pthread_mutex_unlock(&r->died_mutex);
		}
		pthread_mutex_unlock(&r->meal_check);
		(*i)++;
		usleep(500);
	}
}

void	death_checker(t_simulation *r, int *i, t_philosopher *p)
{
	while (!r->died && !r->all_ate)
	{
		*i = 0;
		check_if_philo_died(r, i, p);
		if (r->nb_eat != -1)
		{
			pthread_mutex_lock(&r->meal_check);
			*i = 0;
			while (*i < r->nb_philo
				&& r->philosophers[*i].nb_eaten >= r->nb_eat)
				(*i)++;
			if (*i == r->nb_philo)
				r->all_ate = 1;
			pthread_mutex_unlock(&r->meal_check);
		}
	}
}

int	launch_philosophers(t_simulation *rules, int i)
{
	while (i < rules->nb_philo)
	{
		pthread_mutex_lock(&rules->meal_check);
		rules->philosophers[i].t_since_last_meal = rules->first_current_time;
		pthread_mutex_unlock(&rules->meal_check);
		if (pthread_create(&rules->philosophers[i].thread_id, NULL,
				philo_routine, &rules->philosophers[i]))
			return (1);
		i++;
	}
	return (0);
}

long long	current_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	single_philo(t_simulation *r, t_philosopher *p)
{
	if (r->nb_philo == 1)
	{
		pthread_mutex_lock(&r->forks[0]);
		print_status(r, p->philo_id, FORK);
		while (1)
		{
			pthread_mutex_lock(&r->died_mutex);
			if (r->died)
			{
				pthread_mutex_unlock(&r->died_mutex);
				break ;
			}
			pthread_mutex_unlock(&r->died_mutex);
			usleep(500);
		}
		pthread_mutex_unlock(&r->forks[0]);
		return ;
	}
}
