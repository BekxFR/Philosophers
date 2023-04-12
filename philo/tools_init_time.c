/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_init_time.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 11:13:07 by chillion          #+#    #+#             */
/*   Updated: 2023/01/11 12:27:10 by chillion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_init_arg(t_d *var, int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc && argv[i])
	{
		if (ft_strlen(argv[i]) == 0 || ft_digit_only_arg(argv[i]) == -1)
		{
			write(2, "Error format with arg on \"\" : \"", 32);
			ft_putstr_fd(argv[i], 2);
			write(2, "\".\nNeed only digit on INT_MAX range.\n", 38);
			return (1);
		}
		i++;
	}
	i = 0;
	var->nbr_of_philo = ft_digit_only_arg(argv[1]);
	var->t_die = ft_digit_only_arg(argv[2]);
	var->t_eat = ft_digit_only_arg(argv[3]);
	var->t_sleep = ft_digit_only_arg(argv[4]);
	if (argc == 6)
	{
		var->format = 1;
		var->nbr_each_philo_eat = ft_digit_only_arg(argv[5]);
	}
	return (0);
}

int	ft_init_var(t_d *var)
{
	var->t_die = 0;
	var->t_eat = 0;
	var->t_sleep = 0;
	var->nbr_of_philo = 0;
	var->dead = 0;
	var->nbr_each_philo_eat = -1;
	var->status = 0;
	var->format = 0;
	return (0);
}

int	ft_init_philo(t_d *var)
{
	int	i;

	i = 0;
	if (var->nbr_of_philo > 300)
		return (ft_putstr_fd("Number of philo is too high : over 300\n", 2), 1);
	while (i < var->nbr_of_philo)
	{
		var->philo[i].pos = i;
		var->philo[i].index = var->philo[i].pos + 1;
		var->philo[i].t_die = var->t_die;
		var->philo[i].t_eat = var->t_eat;
		var->philo[i].t_n_eat = (ft_return_millisec() + var->t_die);
		var->philo[i].nbr_eat = 0;
		var->philo[i].need_eat = var->nbr_each_philo_eat;
		var->philo[i].t_sleep = var->t_sleep;
		var->philo[i].fleft = i;
		var->philo[i].data = var;
		if (i == 0)
			var->philo[i].fright = var->nbr_of_philo - 1;
		else
			var->philo[i].fright = i - 1;
		i++;
	}
	return (0);
}

int	ft_usleep_milli(t_p *philo, int ref)
{
	long int	milliseconds;
	long int	interval;
	long int	time;

	time = 0;
	milliseconds = ft_return_millisec();
	while (status_dead(philo->data) == 0 && time < ref)
	{
		if (status_eat(philo, ft_return_millisec()) != 0)
			return (1);
		interval = ft_return_millisec();
		time = interval - milliseconds;
		usleep(50);
	}
	return (0);
}

long int	ft_return_millisec(void)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, 0) == -1)
		return (write(2, "gettimeofday Error\n", 20), -1);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}
