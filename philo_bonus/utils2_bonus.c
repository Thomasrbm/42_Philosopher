/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 12:10:20 by throbert          #+#    #+#             */
/*   Updated: 2025/03/27 18:04:13 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	sim_stopped(t_simulation *r)
{
	int	stopped;

	sem_wait(r->finished_sem);
	stopped = r->finished;
	sem_post(r->finished_sem);
	return (stopped);
}

void	init_local_meal_sem(t_simulation *r, int id)
{
	char	*number_str;
	char	*sem_name;

	number_str = ft_itoa(id);
	if (!number_str)
		exit(1);
	sem_name = ft_strjoin("/local_meal_sem_", number_str);
	free(number_str);
	if (!sem_name)
		exit(1);
	sem_unlink(sem_name);
	r->local_meal_sem = sem_open(sem_name, O_CREAT, 0644, 1);
	if (r->local_meal_sem == SEM_FAILED)
	{
		free(sem_name);
		exit(1);
	}
	free(sem_name);
}

int	is_valid_arg(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (!(argv[i][j] >= '0' && argv[i][j] <= '9'))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}
