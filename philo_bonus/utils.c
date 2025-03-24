/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:21:13 by throbert          #+#    #+#             */
/*   Updated: 2025/03/14 12:32:05 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_simulation *rules, int id, char *string)
{
	pthread_mutex_lock(&(rules->printing));
	if (!(rules->died))
	{
		printf(BOLD "%5lli " RESET, current_time() - rules->first_current_time);
		printf("%3i ", id + 1);
		printf("%s" RESET "\n", string);
	}
	pthread_mutex_unlock(&(rules->printing));
	return ;
}

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
		str++;
	}
	if (result * sign > INT_MAX || result * sign < INT_MIN || result * sign < 0)
		return (-2);
	return (result * sign);
}

long long	current_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	better_sleep(long long time, t_simulation *rules)
{
	long long	start;
	int			died;

	start = current_time();
	while (1)
	{
		pthread_mutex_lock(&rules->meal_check);
		died = rules->died;
		pthread_mutex_unlock(&rules->meal_check);
		if (died)
			break ;
		if (current_time() - start >= time)
			break ;
		usleep(100);
	}
}

void	*philo_routine(void *void_philosopher)
{
	int				i;
	t_philosopher	*philo;
	t_simulation	*rules;

	i = 0;
	philo = (t_philosopher *)void_philosopher;
	rules = philo->rules;
	if (philo->philo_id % 2)
		usleep(15000);
	while (!(rules->died))
	{
		philo_eats(philo);
		if (rules->all_ate)
			break ;
		print_status(rules, philo->philo_id, SLEEP);
		better_sleep(rules->time_sleep, rules);
		print_status(rules, philo->philo_id, THINK);
		i++;
	}
	return (NULL);
}
