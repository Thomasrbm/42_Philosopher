/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 10:29:04 by throbert          #+#    #+#             */
/*   Updated: 2025/03/28 06:33:55 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_rules(t_simulation *r, char **argv)
{
	r->nb_philo = ft_atol(argv[1]);
	r->time_die = ft_atol(argv[2]);
	r->time_eat = ft_atol(argv[3]);
	r->time_sleep = ft_atol(argv[4]);
	if (argv[5])
		r->nb_to_eat = ft_atol(argv[5]);
	else
		r->nb_to_eat = -1;
	r->eaten = 0;
	r->exit_status = 0;
	if (r->nb_philo <= 0 || r->nb_philo > 200 || r->time_die <= 0
		|| r->time_eat <= 0 || r->time_sleep <= 0 || (argv[5]
			&& r->nb_to_eat <= 0))
		return (0);
	return (1);
}

static int	init_semaphores(t_simulation *r)
{
	sem_unlink("forks");
	sem_unlink("write");
	sem_unlink("diverse_updt");
	sem_unlink("dead");
	r->dead = sem_open("dead", O_CREAT, 0644, 1);
	r->forks = sem_open("forks", O_CREAT, 0644, r->nb_philo);
	r->write_sem = sem_open("write", O_CREAT, 0644, 1);
	r->diverse_updt = sem_open("diverse_updt", O_CREAT, 0644, 1);
	if (r->forks == SEM_FAILED || r->write_sem == SEM_FAILED
		|| r->diverse_updt == SEM_FAILED || r->dead == SEM_FAILED)
	{
		return (0);
	}
	return (1);
}

int	init_simulation(t_simulation *r, char **argv)
{
	if (!is_valid_arg(argv) || !init_rules(r, argv))
	{
		write(2, ERROR, sizeof(ERROR));
		return (0);
	}
	if (!init_semaphores(r))
	{
		write(2, "Error: Semaphore init failed\n", 30);
		return (0);
	}
	r->pids = malloc(sizeof(pid_t) * r->nb_philo);
	memset(r->pids, 0, sizeof(pid_t) * r->nb_philo);
	if (!r->pids)
	{
		write(2, "Error: Malloc failed\n", 21);
		return (0);
	}
	r->start_time = get_time();
	return (1);
}

int	main(int argc, char **argv)
{
	t_simulation	r;
	int				id;

	if (argc != 5 && argc != 6)
		return (write(2, "Error: Wrong arg count\n", 24));
	if (!init_simulation(&r, argv))
		return (1);
	id = 0;
	while (id < r.nb_philo)
	{
		r.pids[id] = fork();
		if (r.pids[id] < 0)
			return (write(2, "Error: Fork failed\n", 19));
		if (r.pids[id] == 0)
		{
			if (id % 2 == 0)
				usleep(500);
			philo_life(id, &r);
			exit(0);
		}
		id++;
	}
	wait_philos(&r);
	return (0);
}
