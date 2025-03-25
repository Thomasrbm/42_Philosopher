/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:19:05 by throbert          #+#    #+#             */
/*   Updated: 2025/03/25 22:21:58 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	start_simulation(t_simulation *rules)
{
	int	i;

	rules->first_current_time = current_time();
	i = 0;
	if (launch_philosophers(rules, i))
		return (1);
	death_checker(rules, &i, rules->philosophers);
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

void	philo_eats(t_philosopher *p)
{
	t_simulation	*r;

	r = p->rules;
	if (r->nb_philo == 1)
	{
		pthread_mutex_lock(&r->forks[0]);
		print_status(r, p->philo_id, FORK);
		while (!r->died)
			usleep(500);
		pthread_mutex_unlock(&r->forks[0]);
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
		|| pthread_mutex_init(&r->meal_check, NULL))
		return (0);
	return (1);
}

static void	init_philosophers(t_simulation *rules)
{
	int	i;
	int	left;
	int	right;
	int	temp;

	i = 0;
	while (i < rules->nb_philo)
	{
		left = i;
		right = (i + 1) % rules->nb_philo;
		if (left > right)
		{
			temp = left;
			left = right;
			right = temp;
		}
		rules->philosophers[i] = (t_philosopher){i, 0, left, right, 0, rules,
			(pthread_t){0}};
		i++;
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

// Simplifier les fonction

// rename rules.philosophers  em philo_id
