/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:42:32 by throbert          #+#    #+#             */
/*   Updated: 2025/03/21 12:43:41 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death_status(t_simulation *rules)
{
	int	died;

	pthread_mutex_lock(&rules->death_mutex);
	died = rules->died;
	pthread_mutex_unlock(&rules->death_mutex);
	return (died);
}

// time to eat maj au debut sinon pourrait dire qu il a
// pas manger depuis trop longtemps alors qu il est en train
void	update_meal_info(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->rules->last_meal_checker);
	philo->time_of_last_meal = current_time();
	philo->nb_meal_eaten++;
	pthread_mutex_unlock(&philo->rules->last_meal_checker);
}
