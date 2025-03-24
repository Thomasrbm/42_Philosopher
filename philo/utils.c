/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:21:13 by throbert          #+#    #+#             */
/*   Updated: 2025/03/23 18:56:51 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_simulation *rules, int philo_id_number, char *string)
{
	int	died;

	pthread_mutex_lock(&(rules->print_status));
	pthread_mutex_lock(&rules->death_mutex);
	died = rules->died;
	pthread_mutex_unlock(&rules->death_mutex);
	if (!died)
	{
		printf(RESET_PRECEDENT_STYLE BOLD "%5lli ", current_time()
			- rules->first_current_time);
		printf("%3i " RESET_PRECEDENT_STYLE, philo_id_number + 1);
		printf("%s\n" RESET_PRECEDENT_STYLE, string);
	}
	pthread_mutex_unlock(&(rules->print_status));
}

// forbid overflow of int bc return -1 (error in init_rules)
// or more than max int
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

long long	current_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	better_usleep(long long time, t_simulation *rules)
{
	long long	begin_time;
	int			died;

	begin_time = current_time();
	while (1)
	{
		pthread_mutex_lock(&rules->death_mutex);
		died = rules->died;
		pthread_mutex_unlock(&rules->death_mutex);
		if (died)
			break ;
		if (current_time() - begin_time >= time)
			break ;
		usleep(1);
	}
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
