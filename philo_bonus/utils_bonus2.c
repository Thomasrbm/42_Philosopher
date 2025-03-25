/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 23:24:00 by throbert          #+#    #+#             */
/*   Updated: 2025/03/24 03:52:21 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	single_life(int id, t_simulation *sim)
{
	sem_wait(sim->write_sem);
	printf(BOLD "%5ld %3d %s\n" RESET, get_time() - sim->start_time, id + 1,
		FORK);
	sem_post(sim->write_sem);
	precise_sleep(sim->time_die);
	sem_wait(sim->write_sem);
	printf(BOLD "%5ld %3d %s\n" RESET, get_time() - sim->start_time, id + 1,
		DIE);
	sem_post(sim->write_sem);
	exit(0);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
