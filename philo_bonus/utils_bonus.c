/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 10:46:29 by throbert          #+#    #+#             */
/*   Updated: 2025/03/23 12:40:00 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long	ft_atol(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		if (result * sign > INT_MAX)
			return (-1);
		if (result * sign < INT_MIN)
			return (-1);
		str++;
	}
	return (result * sign);
}

void	precise_sleep(long milliseconds)
{
	long	start;

	start = get_time();
	while (get_time() - start < milliseconds)
		usleep(100);
}

void	kill_remaining(t_simulation *sim, pid_t pid)
{
	int	i;

	i = 0;
	while (i < sim->nb_philo)
	{
		if (sim->pids[i] != pid && sim->pids[i] > 0)
		{
			kill(sim->pids[i], SIGKILL);
			waitpid(sim->pids[i], NULL, 0);
		}
		i++;
	}
}

void	cleanup_semaphores(t_simulation *sim)
{
	sem_close(sim->forks);
	sem_close(sim->write_sem);
	sem_close(sim->meal_sem);
	sem_close(sim->death_sem);
	sem_close(sim->finished_sem);
	sem_unlink("forks");
	sem_unlink("write");
	sem_unlink("meal_sem");
	sem_unlink("death_sem");
	sem_unlink("finished_sem");
}

void	wait_philos(t_simulation *sim)
{
	int		status;
	pid_t	pid;
	int		died;

	died = 0;
	while (!died)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		if (WIFEXITED(status))
		{
			if (WEXITSTATUS(status) == 1)
			{
				kill_remaining(sim, pid);
				died = 1;
			}
		}
	}
	cleanup_semaphores(sim);
}
