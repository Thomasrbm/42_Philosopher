/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 23:08:41 by throbert          #+#    #+#             */
/*   Updated: 2025/03/24 02:31:20 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_philo_forks(t_simulation *rules, int i)
{
	if (i % 2 == 0)
	{
		rules->philosophers[i].left_fork_id = i;
		rules->philosophers[i].right_fork_id = (i + 1) % rules->nb_of_philo;
	}
	else
	{
		rules->philosophers[i].left_fork_id = (i + 1) % rules->nb_of_philo;
		rules->philosophers[i].right_fork_id = i;
	}
	return (0);
}

int	init_philosophers(t_simulation *rules)
{
	int	i;

	i = -1;
	rules->forks = malloc(sizeof(pthread_mutex_t) * rules->nb_of_philo);
	rules->philosophers = malloc(sizeof(t_philosopher) * rules->nb_of_philo);
	if (!rules->forks || !rules->philosophers)
	{
		free(rules->forks);
		free(rules->philosophers);
		return (write(2, "Error: Malloc failed\n", 21));
	}
	while (++i < rules->nb_of_philo)
	{
		if (pthread_mutex_init(&rules->forks[i], NULL))
			return (write(2, "Error: Fork mutex\n", 19));
		rules->philosophers[i] = (t_philosopher){i, 0, i, (i + 1)
			% rules->nb_of_philo, 0, rules, (pthread_t){0}};
		init_philo_forks(rules, i);
	}
	return (0);
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

int	main(int argc, char **argv)
{
	t_simulation	rules;

	if (argc != 5 && argc != 6)
		return (write(2, "Error: Wrong arg count\n", 24));
	if (!is_valid_arg(argv) || !init_rules(&rules, argv))
		return (write(2, ERROR, 62));
	if (init_philosophers(&rules))
		return (1);
	if (start_simulation(&rules))
		return (write(2, "Error: Thread creation failed\n", 31));
	free(rules.philosophers);
	free(rules.forks);
	return (0);
}
