/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 01:20:07 by throbert          #+#    #+#             */
/*   Updated: 2025/04/01 10:42:36 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	better_sleep(long long time, t_simulation *rules)
{
	long long	start;
	int			died;

	start = current_time();
	while (1)
	{
		pthread_mutex_lock(&rules->meal_check);
		died = rules->died;
		pthread_mutex_unlock(&rules->meal_check);
		if (died)
			break ;
		if (current_time() - start >= time)
			break ;
		usleep(500);
	}
}

void	philo_eats(t_philosopher *p)
{
	t_simulation	*r;

	r = p->rules;
	if (r->nb_philo == 1)
	{
		single_philo(r, p);
		return ;
	}
	pthread_mutex_lock(&r->forks[p->left_fork_id]);
	print_status(r, p->philo_id, FORK);
	pthread_mutex_lock(&r->forks[p->right_fork_id]);
	print_status(r, p->philo_id, FORK);
	pthread_mutex_lock(&r->meal_check);
	print_status(r, p->philo_id, EAT);
	p->t_since_last_meal = current_time();
	p->nb_eaten++;
	pthread_mutex_unlock(&r->meal_check);
	better_sleep(r->time_eat, r);
	pthread_mutex_unlock(&r->forks[p->left_fork_id]);
	pthread_mutex_unlock(&r->forks[p->right_fork_id]);
}

void	*philo_routine(void *void_philosopher)
{
	t_philosopher	*philo;
	t_simulation	*rules;

	philo = (t_philosopher *)void_philosopher;
	rules = philo->rules;
	if (philo->philo_id % 2)
		usleep(500);
	while (1)
	{
		if (check_simulation_end(rules))
			break ;
		philo_eats(philo);
		if (check_simulation_end(rules))
			break ;
		better_sleep(rules->time_sleep, rules);
		if (check_simulation_end(rules))
			break ;
		print_status(rules, philo->philo_id, SLEEP);
		if (check_simulation_end(rules))
			break ;
		print_status(rules, philo->philo_id, THINK);
	}
	return (NULL);
}
