/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: throbert <throbert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 11:07:44 by throbert          #+#    #+#             */
/*   Updated: 2025/03/14 11:32:34 by throbert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

int	get_length_without_leading_zeros(const char *str)
{
	int	len;

	while (*str == '0')
		str++;
	len = 0;
	while (str[len])
		len++;
	return (len);
}

int	is_valid_arg(char **argv)
{
	int	i;
	int	j;
	int	check;

	i = 1;
	check = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (!ft_isdigit(argv[i][j]))
				return (0);
			j++;
		}
		if (get_length_without_leading_zeros(argv[i]) > 10)
			return (0);
		i++;
	}
	return (1);
}
