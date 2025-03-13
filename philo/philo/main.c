/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 23:08:41 by throbert          #+#    #+#             */
/*   Updated: 2025/03/03 00:22:45 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_simulation	rules;
	int				i;

	if (argc != 5 && argc != 6)
		return (write(2, "Error: Wrong arg count\n", 24));
	if (!is_valid_arg(argv) || !init_rules(&rules, argv))
		return (write(2, ERROR, 62));
	i = 0;
	rules.forks = malloc(sizeof(pthread_mutex_t) * rules.nb_of_philo);
	rules.philosophers = malloc(sizeof(t_philosopher) * rules.nb_of_philo);
	while (i < rules.nb_of_philo)
	{
		if (pthread_mutex_init(&rules.forks[i], NULL))
			return (write(2, "Error: Fork mutex\n", 19));
		rules.philosophers[i] = (t_philosopher){i, 0, i, (i + 1)
			% rules.nb_of_philo, 0, &rules, (pthread_t){0}};
		i++;
	}
	if (start_simulation(&rules))
		return (write(2, "Error: Thread creation failed\n", 31));
	free(rules.philosophers);
	free(rules.forks);
	return (0);
}
