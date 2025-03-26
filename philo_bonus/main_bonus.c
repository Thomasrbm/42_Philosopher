/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 10:29:04 by throbert          #+#    #+#             */
/*   Updated: 2025/03/26 00:46:50 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_rules(t_simulation *sim, char **argv)
{
	sim->nb_philo = ft_atol(argv[1]);
	sim->time_die = ft_atol(argv[2]);
	sim->time_eat = ft_atol(argv[3]);
	sim->time_sleep = ft_atol(argv[4]);
	if (argv[5])
		sim->nb_eat = ft_atol(argv[5]);
	else
		sim->nb_eat = -1;
	sim->eaten = 0;
	if (sim->nb_philo <= 0 || sim->nb_philo > 200 || sim->time_die <= 0
		|| sim->time_eat <= 0 || sim->time_sleep <= 0 || (argv[5]
			&& sim->nb_eat <= 0))
		return (0);
	return (1);
}

static int	init_semaphores(t_simulation *sim)
{
	sem_unlink("forks");
	sem_unlink("write");
	sem_unlink("meal_sem");
	sem_unlink("death_sem");
	sem_unlink("finished_sem");
	sim->forks = sem_open("forks", O_CREAT, 0644, sim->nb_philo);
	sim->write_sem = sem_open("write", O_CREAT, 0644, 1);
	sim->meal_sem = sem_open("meal_sem", O_CREAT, 0644, 1);
	sim->death_sem = sem_open("death_sem", O_CREAT, 0644, 1);
	sim->finished_sem = sem_open("finished_sem", O_CREAT, 0644, 1);
	if (sim->forks == SEM_FAILED || sim->write_sem == SEM_FAILED
		|| sim->meal_sem == SEM_FAILED || sim->death_sem == SEM_FAILED
		|| sim->finished_sem == SEM_FAILED)
	{
		return (0);
	}
	return (1);
}

int	init_simulation(t_simulation *sim, char **argv)
{
	if (!is_valid_arg(argv) || !init_rules(sim, argv))
	{
		write(2, ERROR, sizeof(ERROR));
		return (0);
	}
	if (!init_semaphores(sim))
	{
		write(2, "Error: Semaphore init failed\n", 30);
		return (0);
	}
	sim->pids = malloc(sizeof(pid_t) * sim->nb_philo);
	if (!sim->pids)
	{
		write(2, "Error: Malloc failed\n", 21);
		return (0);
	}
	sim->start_time = get_time();
	return (1);
}

int	init_philo(t_simulation *sim, int id, pid_t pid)
{
	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		if (sim->nb_philo % 2 == 0)
		{
			if (id % 2 == 0)
				usleep(15000);
		}
		philo_life(id, sim);
		exit(0);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_simulation	sim;
	int				id;
	pid_t			pid;

	pid = 0;
	if (argc != 5 && argc != 6)
		return (write(2, "Error: Wrong arg count\n", 24));
	if (!init_simulation(&sim, argv))
		return (1);
	id = 0;
	while (id < sim.nb_philo)
	{
		if (init_philo(&sim, id, pid))
			return (write(2, "Error: Fork failed\n", 19));
		sim.pids[id++] = pid;
	}
	wait_philos(&sim);
	free(sim.pids);
	return (0);
}

// pair = super sinon pas ducout

// f sanitiwe  ./philo_bonus 200 800 50 50 2
