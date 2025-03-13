/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:19:05 by throbert          #+#    #+#             */
/*   Updated: 2025/03/04 04:51:44 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	death_checker(t_simulation *r, int *i, t_philosopher *p)
{
	while (!r->died && !r->all_ate)
	{
		*i = 0;
		while (*i < r->nb_of_philo && !r->died)
		{
			pthread_mutex_lock(&r->last_meal_checker);
			if (current_time() - p[*i].time_of_last_meal > r->time_death)
			{
				print_status(r, *i, DIE);
				r->died = 1;
			}
			pthread_mutex_unlock(&r->last_meal_checker);
			(*i)++;
			usleep(100);
		}
		if (r->nb_to_eat_max != -1)
		{
			*i = 0;
			while (*i < r->nb_of_philo
				&& p[*i].nb_meal_eaten >= r->nb_to_eat_max)
				(*i)++;
			if (*i == r->nb_of_philo)
				r->all_ate = 1;
		}
	}
}

// time to eat maj au debut sinon pourrait dire qu il a pas manger depuis trop longtemps alors qu il est en train
void	philo_eats(t_philosopher *curr_philo)
{
	if (curr_philo->rules->nb_of_philo == 1)
	{
		pthread_mutex_lock(&curr_philo->rules->forks[0]);
		print_status(curr_philo->rules, curr_philo->philo_id_number, FORK);
		while (!curr_philo->rules->died)
			usleep(200);
		pthread_mutex_unlock(&curr_philo->rules->forks[0]);
		return ;
	}
	pthread_mutex_lock(&curr_philo->rules->forks[curr_philo->left_fork_id]);
	print_status(curr_philo->rules, curr_philo->philo_id_number, FORK);
	pthread_mutex_lock(&curr_philo->rules->forks[curr_philo->right_fork_id]);
	print_status(curr_philo->rules, curr_philo->philo_id_number, FORK);
	pthread_mutex_lock(&curr_philo->rules->last_meal_checker);
	print_status(curr_philo->rules, curr_philo->philo_id_number, EAT);
	curr_philo->time_of_last_meal = current_time();
	pthread_mutex_unlock(&curr_philo->rules->last_meal_checker);
	better_usleep(curr_philo->rules->time_eat, curr_philo->rules);
	(curr_philo->nb_meal_eaten)++;
	pthread_mutex_unlock(&curr_philo->rules->forks[curr_philo->left_fork_id]);
	pthread_mutex_unlock(&curr_philo->rules->forks[curr_philo->right_fork_id]);
}

void	*philo_routine(void *void_philosopher)
{
	t_philosopher	*philo;
	t_simulation	*rules;

	philo = (t_philosopher *)void_philosopher;
	rules = philo->rules;
	if (philo->philo_id_number % 2)
		usleep(15000);
	while (!rules->died)
	{
		if (rules->nb_to_eat_max != -1
			&& philo->nb_meal_eaten >= rules->nb_to_eat_max)
			break ;
		philo_eats(philo);
		if (rules->all_ate)
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
		if (pthread_create(&rules->philosophers[i].thread_id, NULL,
				philo_routine, &rules->philosophers[i]))
			return (1);
		rules->philosophers[i].time_of_last_meal = current_time();
		i++;
	}
	if (rules->philosophers > 0)
		death_checker(rules, &i, rules->philosophers);
	i = 0;
	while (i < rules->nb_of_philo)
		pthread_join(rules->philosophers[i++].thread_id, NULL);
	i = 0;
	while (i < rules->nb_of_philo)
		pthread_mutex_destroy(&rules->forks[i++]);
	pthread_mutex_destroy(&rules->print_status);
	pthread_mutex_destroy(&rules->last_meal_checker);
	return (0);
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
	if (pthread_mutex_init(&rules->print_status, NULL))
		return (0);
	if (pthread_mutex_init(&rules->last_meal_checker, NULL))
		return (0);
	return (1);
}


// JUSTIFIER pk CHAQUE POINTEURS ??????//

// pk usleep 15000 oblig pour pair ?
