/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:21:13 by throbert          #+#    #+#             */
/*   Updated: 2025/03/29 01:22:35 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_simulation *rules, int id, char *string)
{
	pthread_mutex_lock(&(rules->printing));
	pthread_mutex_lock(&(rules->died_mutex));
	if (!(rules->died))
	{
		printf(BOLD "%5lli " RESET, current_time() - rules->first_current_time);
		printf("%3i ", id + 1);
		printf("%s" RESET "\n", string);
	}
	pthread_mutex_unlock(&(rules->printing));
	pthread_mutex_unlock(&(rules->died_mutex));
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
