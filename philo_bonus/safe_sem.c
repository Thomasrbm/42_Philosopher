/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_sem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 00:40:37 by throbert          #+#    #+#             */
/*   Updated: 2025/03/28 01:54:03 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	safe_death(long current_time, int id, t_simulation *r)
{
	sem_wait(r->finished_sem);
	if (!r->finished)
	{
		r->finished = 1;
		r->time_of_death = current_time;
		printf(BOLD "%5ld %3d %s\n" RESET, current_time - r->start_time,
			id, DIE);
		r->exit_status = 1;
	}
	else
		sem_post(r->finished_sem);
}

void	safe_print(long timestamp, char *msg, int id, t_simulation *r)
{
	sem_wait(r->write_sem);
	if (!sim_stopped(r))
		printf(BOLD "%5ld %3d %s\n" RESET, timestamp, id, msg);
	sem_post(r->write_sem);
}

void	safe_sleep_print(t_simulation *r, int id, char *msg, long duration)
{
	long	start;

	if (sim_stopped(r))
		return ;
	start = get_time();
	precise_sleep(duration, r);
	if (!sim_stopped(r))
		safe_print(start - r->start_time, msg, id + 1, r);
}
