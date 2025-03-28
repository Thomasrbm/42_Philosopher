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

void	precise_sleep(long milliseconds, t_simulation *r)
{
	long	start;

	start = get_time();
	while (get_time() - start < milliseconds)
	{
		if (sim_stopped(r))
			break ;
		usleep(1);
	}
}

void	kill_remaining(t_simulation *r, pid_t pid)
{
	int	i;

	i = 0;
	while (i < r->nb_philo)
	{
		if (r->pids[i] != pid && r->pids[i] > 0)
			kill(r->pids[i], SIGKILL);
		i++;
	}
}

void	cleanup_semaphores(t_simulation *r)
{
	sem_unlink("forks");
	sem_unlink("write");
	sem_unlink("meal_sem");
	sem_unlink("death_sem");
	sem_unlink("finished_sem");
	sem_unlink("any_death_sem");
	sem_close(r->any_death_sem);
	sem_close(r->forks);
	sem_close(r->write_sem);
	sem_close(r->meal_sem);
	sem_close(r->death_sem);
	sem_close(r->finished_sem);
}

void	wait_philos(t_simulation *r)
{
	int		status;
	pid_t	pid;
	int		i;

	i = 0;
	status = 0;
	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		{
			kill_remaining(r, pid);
			sem_post(r->finished_sem);
			sem_post(r->write_sem);
			sem_post(r->meal_sem);
			sem_post(r->death_sem);
			sem_post(r->any_death_sem);
			break ;
		}
		else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
			i++;
		if (i == r->nb_philo)
			break ;
	}
	i = r->nb_philo;
	while (i-- > 0)
		sem_post(r->forks);
	sem_post(r->write_sem);
	cleanup_semaphores(r);
	free(r->pids);
}
