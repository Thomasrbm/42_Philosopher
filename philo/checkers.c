/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 21:45:43 by throbert          #+#    #+#             */
/*   Updated: 2025/04/01 06:07:44 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_simulation_end(t_simulation *rules)
{
	pthread_mutex_lock(&rules->died_mutex);
	pthread_mutex_lock(&rules->finish_mutex);
	if (rules->died || rules->all_ate)
	{
		pthread_mutex_unlock(&rules->died_mutex);
		pthread_mutex_unlock(&rules->finish_mutex);
		return (1);
	}
	pthread_mutex_unlock(&rules->died_mutex);
	pthread_mutex_unlock(&rules->finish_mutex);
	return (0);
}

void	check_if_philo_died(t_simulation *r, int *i, t_philosopher *p)
{
	while (*i < r->nb_philo)
	{
		pthread_mutex_lock(&r->meal_check);
		if (r->died)
		{
			pthread_mutex_unlock(&r->meal_check);
			break ;
		}
		if (current_time() - p[*i].t_since_last_meal > r->time_death)
		{
			print_status(r, *i, DIE);
			pthread_mutex_lock(&r->died_mutex);
			r->died = 1;
			pthread_mutex_unlock(&r->died_mutex);
			pthread_mutex_unlock(&r->meal_check);
			break ;
		}
		pthread_mutex_unlock(&r->meal_check);
		(*i)++;
		usleep(500);
	}
}

static int	check_all_ate(t_simulation *r)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&r->meal_check);
	while (i < r->nb_philo)
	{
		if (r->philosophers[i].nb_eaten < r->nb_eat)
			break ;
		i++;
	}
	pthread_mutex_unlock(&r->meal_check);
	return (i == r->nb_philo);
}

void	death__meal_checker(t_simulation *r, int *i, t_philosopher *p)
{
	while (1)
	{
		pthread_mutex_lock(&r->finish_mutex);
		if (r->died || r->all_ate)
		{
			pthread_mutex_unlock(&r->finish_mutex);
			break ;
		}
		pthread_mutex_unlock(&r->finish_mutex);
		*i = 0;
		check_if_philo_died(r, i, p);
		if (r->nb_eat != -1 && check_all_ate(r))
		{
			pthread_mutex_lock(&r->finish_mutex);
			r->all_ate = 1;
			pthread_mutex_unlock(&r->finish_mutex);
		}
		usleep(500);
	}
}
