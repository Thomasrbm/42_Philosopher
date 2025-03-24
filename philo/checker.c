/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:41:09 by throbert          #+#    #+#             */
/*   Updated: 2025/03/24 02:29:38 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_philo_status(t_simulation *r, int i, t_philosopher *p)
{
	long long	last_meal;

	pthread_mutex_lock(&r->last_meal_checker);
	last_meal = current_time() - p[i].time_of_last_meal;
	pthread_mutex_unlock(&r->last_meal_checker);
	if (last_meal > r->time_death)
	{
		print_status(r, i, DIE);
		pthread_mutex_lock(&r->death_mutex);
		r->died = 1;
		pthread_mutex_unlock(&r->death_mutex);
	}
}

int	check_end_conditions(t_simulation *r)
{
	pthread_mutex_lock(&r->death_mutex);
	if (r->died || r->all_ate)
	{
		pthread_mutex_unlock(&r->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&r->death_mutex);
	return (0);
}

void	check_all_philos_ate(t_simulation *r, int *i, t_philosopher *p)
{
	int	all_philos_ate;

	if (r->nb_to_eat_max != -1)
	{
		all_philos_ate = 1;
		*i = -1;
		while (++(*i) < r->nb_of_philo)
		{
			pthread_mutex_lock(&r->last_meal_checker);
			if (p[*i].nbmeal_eaten < r->nb_to_eat_max)
				all_philos_ate = 0;
			pthread_mutex_unlock(&r->last_meal_checker);
		}
		if (all_philos_ate)
		{
			pthread_mutex_lock(&r->death_mutex);
			r->all_ate = 1;
			pthread_mutex_unlock(&r->death_mutex);
		}
	}
}

void	death_checker(t_simulation *r, int *i, t_philosopher *p)
{
	while (1)
	{
		if (check_end_conditions(r))
			break ;
		check_all_philos_ate(r, i, p);
		*i = -1;
		while (++(*i) < r->nb_of_philo)
			check_philo_status(r, *i, p);
		usleep(1000);
	}
}

int	check_death_status(t_simulation *rules)
{
	int	died;

	pthread_mutex_lock(&rules->death_mutex);
	died = rules->died;
	pthread_mutex_unlock(&rules->death_mutex);
	return (died);
}
