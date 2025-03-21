/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 23:08:41 by throbert          #+#    #+#             */
/*   Updated: 2025/03/21 12:36:33 by throbert         ###   ########.fr       */
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
