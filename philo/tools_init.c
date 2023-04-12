/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 11:12:58 by chillion          #+#    #+#             */
/*   Updated: 2023/01/09 18:30:09 by chillion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_putstr_fd(char *str, int fd)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
}

int	ft_digit_only_arg(char *str)
{
	long int	i;
	long int	nbr;

	i = 0;
	nbr = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (-1);
		nbr = (nbr * 10) + (str[i] - 48);
		i++;
	}
	if (nbr > INT_MAX)
		return (-1);
	return (nbr);
}

int	ft_strlen(char *str)
{
	long int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	main_thread_wait(t_p *philo)
{
	if (philo->index % 2)
		usleep(250);
	else
		usleep(250);
}

int	main_thread_tools(t_d *var, t_p *philo)
{
	pthread_mutex_lock(&var->lockeat);
	philo->t_n_eat = (var->begin + (long int)var->t_die);
	pthread_mutex_unlock(&var->lockeat);
	if (var->nbr_of_philo == 1)
		return (ft_usleep_milli(philo, (philo->t_die)), 1);
	if (var->format == 1)
	{
		if (philo->need_eat <= 0)
			return (1);
	}
	return (0);
}
