/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_sem.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 00:40:37 by throbert          #+#    #+#             */
/*   Updated: 2025/04/01 05:48:00 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	safe_print(long timestamp, char *msg, int id, t_simulation *r)
{
	int	dead;

	sem_wait(r->diverse_updt);
	dead = r->dead->__align;
	sem_post(r->diverse_updt);
	sem_wait(r->write_sem);
	if (dead == 1)
		printf(BOLD "%5ld %3d %s\n" RESET, timestamp, id, msg);
	sem_post(r->write_sem);
}

void	safe_sleep_print(t_simulation *r, int id, char *msg, long duration)
{
	long	start;

	start = get_time();
	better_sleep(duration, r);
	safe_print(start - r->start_time, msg, id + 1, r);
}

void	safe_print_dead(long timestamp, char *msg, int id, t_simulation *r)
{
	sem_wait(r->write_sem);
	if (r->dead->__align == 1)
		printf(BOLD "%5ld %3d %s\n" RESET, timestamp - r->start_time, id, msg);
	sem_post(r->write_sem);
}
