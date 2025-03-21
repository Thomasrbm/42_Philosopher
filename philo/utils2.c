/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:41:09 by throbert          #+#    #+#             */
/*   Updated: 2025/03/21 12:42:57 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	check_philo_status(t_simulation *r, int i, t_philosopher *p)
{
	long long	last_meal;
	int			meals;

	pthread_mutex_lock(&r->last_meal_checker);
	last_meal = current_time() - p[i].time_of_last_meal;
	meals = p[i].nb_meal_eaten;
	pthread_mutex_unlock(&r->last_meal_checker);
	if (last_meal > r->time_death)
	{
		print_status(r, i, DIE);
		pthread_mutex_lock(&r->death_mutex);
		r->died = 1;
		pthread_mutex_unlock(&r->death_mutex);
	}
	if (r->nb_to_eat_max != -1 && meals >= r->nb_to_eat_max)
	{
		pthread_mutex_lock(&r->death_mutex);
		r->all_ate = 1;
		pthread_mutex_unlock(&r->death_mutex);
	}
}

void	death_checker(t_simulation *r, int *i, t_philosopher *p)
{
	while (1)
	{
		usleep(1000);
		if (check_end_conditions(r))
			break ;
		*i = -1;
		while (++(*i) < r->nb_of_philo)
			check_philo_status(r, *i, p);
	}
}

int	init_mutexes(t_simulation *rules)
{
	if (pthread_mutex_init(&rules->print_status, NULL))
		return (0);
	if (pthread_mutex_init(&rules->last_meal_checker, NULL))
		return (0);
	if (pthread_mutex_init(&rules->death_mutex, NULL))
		return (0);
	return (1);
}

int	init_rules(t_simulation *rules, char **argv)
{
	rules->nb_of_philo = ft_atol(argv[1]);
	if (rules->nb_of_philo > 200)
		return (0);
	rules->time_death = ft_atol(argv[2]);
	rules->time_eat = ft_atol(argv[3]);
	rules->time_sleep = ft_atol(argv[4]);
	rules->died = 0;
	rules->all_ate = 0;
	if (argv[5])
	{
		rules->nb_to_eat_max = ft_atol(argv[5]);
		if (rules->nb_to_eat_max <= 0)
			return (0);
	}
	else
		rules->nb_to_eat_max = -1;
	if (rules->time_death <= 0 || rules->time_eat <= 0 || rules->time_sleep <= 0
		|| rules->nb_of_philo <= 0)
		return (0);
	if (!init_mutexes(rules))
		return (0);
	return (1);
}
