/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_kill_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 10:46:29 by throbert          #+#    #+#             */
/*   Updated: 2025/03/28 06:34:23 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
	sem_unlink("diverse_updt");
	sem_unlink("dead");
	sem_close(r->dead);
	sem_close(r->forks);
	sem_close(r->write_sem);
	sem_close(r->diverse_updt);
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
			break ;
		}
		else if (WIFEXITED(status) && WEXITSTATUS(status) == 2)
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
