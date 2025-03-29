/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:19:05 by throbert          #+#    #+#             */
/*   Updated: 2025/03/29 01:21:15 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	start_simulation(t_simulation *rules)
{
	int	i;

	rules->first_current_time = current_time();
	i = 0;
	if (launch_philosophers(rules, i))
		return (1);
	death__meal_checker(rules, &i, rules->philosophers);
	i = 0;
	while (i < rules->nb_philo)
	{
		pthread_join(rules->philosophers[i].thread_id, NULL);
		i++;
	}
	i = 0;
	while (i < rules->nb_philo)
	{
		pthread_mutex_destroy(&rules->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&rules->printing);
	return (0);
}

int	init_simulation(t_simulation *r, char **argv)
{
	r->nb_philo = ft_atol(argv[1]);
	if (r->nb_philo <= 0 || r->nb_philo > 200)
		return (0);
	r->time_death = ft_atol(argv[2]);
	r->time_eat = ft_atol(argv[3]);
	r->time_sleep = ft_atol(argv[4]);
	r->died = 0;
	r->all_ate = 0;
	if (argv[5])
	{
		r->nb_eat = ft_atol(argv[5]);
		if (r->nb_eat <= 0)
			return (0);
	}
	else
		r->nb_eat = -1;
	if (r->nb_philo == -2 || r->time_death == -2 || r->time_eat == -2
		|| r->time_sleep == -2 || r->nb_eat == -2)
		return (0);
	if (pthread_mutex_init(&r->printing, NULL)
		|| pthread_mutex_init(&r->meal_check, NULL)
		|| pthread_mutex_init(&r->died_mutex, NULL)
		|| pthread_mutex_init(&r->finish_mutex, NULL))
		return (0);
	return (1);
}

static void	init_philosophers(t_simulation *rules)
{
	int	id;
	int	left;
	int	right;
	int	temp;

	id = 0;
	while (id < rules->nb_philo)
	{
		left = id;
		right = (id + 1) % rules->nb_philo;
		if (left > right)
		{
			temp = left;
			left = right;
			right = temp;
		}
		rules->philosophers[id] = (t_philosopher){id, 0, left, right, 0, rules,
			(pthread_t){0}};
		if (pthread_mutex_init(&rules->forks[id], NULL))
			return ;
		id++;
	}
}

int	main(int argc, char **argv)
{
	t_simulation	rules;

	if (argc != 5 && argc != 6)
		return (write(2, "Error: Wrong arg count\n", 24));
	if (!is_valid_arg(argv) || !init_simulation(&rules, argv))
		return (write(2,
				"One argument is wrong : only positive int and 200 phil max.\n",
				61));
	init_philosophers(&rules);
	if (start_simulation(&rules))
		return (write(2, "Error: Thread creation failed\n", 31));
	return (0);
}
