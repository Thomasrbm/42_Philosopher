/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:19:05 by throbert          #+#    #+#             */
/*   Updated: 2025/03/24 03:12:35 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philosopher *p)
{
	pthread_mutex_lock(&p->rules->forks[p->left_fork_id]);
	print_status(p->rules, p->philo_id_number, FORK);
	pthread_mutex_lock(&p->rules->forks[p->right_fork_id]);
	print_status(p->rules, p->philo_id_number, FORK);
}

void	philo_eats(t_philosopher *curr_philo)
{
	if (curr_philo->rules->nb_of_philo == 1)
	{
		pthread_mutex_lock(&curr_philo->rules->forks[0]);
		print_status(curr_philo->rules, curr_philo->philo_id_number, FORK);
		while (!check_death_status(curr_philo->rules))
			usleep(200);
		pthread_mutex_unlock(&curr_philo->rules->forks[0]);
		return ;
	}
	take_forks(curr_philo);
	pthread_mutex_lock(&curr_philo->rules->last_meal_checker);
	curr_philo->time_of_last_meal = current_time();
	curr_philo->nbmeal_eaten++;
	pthread_mutex_unlock(&curr_philo->rules->last_meal_checker);
	print_status(curr_philo->rules, curr_philo->philo_id_number, EAT);
	better_usleep(curr_philo->rules->time_eat, curr_philo->rules);
	pthread_mutex_unlock(&curr_philo->rules->forks[curr_philo->left_fork_id]);
	pthread_mutex_unlock(&curr_philo->rules->forks[curr_philo->right_fork_id]);
}

int	check_death_or_full(t_philosopher *philo)
{
	int				died;
	int				all_ate;
	t_simulation	*rules;

	rules = philo->rules;
	pthread_mutex_lock(&rules->death_mutex);
	died = rules->died;
	all_ate = rules->all_ate;
	pthread_mutex_unlock(&rules->death_mutex);
	return (died || all_ate);
}

void	*philo_routine(void *void_philosopher)
{
	t_philosopher	*philo;
	t_simulation	*rules;

	philo = (t_philosopher *)void_philosopher;
	rules = philo->rules;
	if (philo->philo_id_number % 2)
		usleep(15000);
	while (!check_death_or_full(philo))
	{
		philo_eats(philo);
		if (check_death_or_full(philo))
			break ;
		print_status(rules, philo->philo_id_number, SLEEP);
		better_usleep(rules->time_sleep, rules);
		print_status(rules, philo->philo_id_number, THINK);
	}
	return (NULL);
}

int	start_simulation(t_simulation *rules)
{
	int	i;

	rules->first_current_time = current_time();
	i = 0;
	while (i < rules->nb_of_philo)
	{
		pthread_mutex_lock(&rules->last_meal_checker);
		rules->philosophers[i].time_of_last_meal = current_time();
		pthread_mutex_unlock(&rules->last_meal_checker);
		if (pthread_create(&rules->philosophers[i].thread_id, NULL,
				philo_routine, &rules->philosophers[i]))
			return (1);
		i++;
	}
	if (rules->nb_of_philo > 0)
		death_checker(rules, &i, rules->philosophers);
	i = 0;
	while (i < rules->nb_of_philo)
		pthread_join(rules->philosophers[i++].thread_id, NULL);
	pthread_mutex_destroy(&rules->print_status);
	pthread_mutex_destroy(&rules->last_meal_checker);
	pthread_mutex_destroy(&rules->death_mutex);
	return (0);
}

// JUSTIFIER pk CHAQUE POINTEURS ??????//

// pk usleep 15000 oblig pour pair ?
