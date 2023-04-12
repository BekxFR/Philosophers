/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_lock.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 10:55:41 by chillion          #+#    #+#             */
/*   Updated: 2023/01/10 18:42:51 by chillion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	status_dead(t_d *var)
{
	pthread_mutex_lock(&(var->lockdead));
	if (var->dead != 0)
	{
		pthread_mutex_unlock(&(var->lockdead));
		return (1);
	}
	pthread_mutex_unlock(&(var->lockdead));
	return (0);
}

int	status_eat(t_p *philo, long int time)
{
	pthread_mutex_lock(&philo->data->lockeat);
	if (time > philo->t_n_eat)
	{
		pthread_mutex_unlock(&philo->data->lockeat);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->lockeat);
	return (0);
}

int	status_end(t_d *var)
{
	pthread_mutex_lock(&var->lockstatus);
	if (var->status != 0)
	{
		pthread_mutex_unlock(&var->lockstatus);
		return (1);
	}
	pthread_mutex_unlock(&var->lockstatus);
	return (0);
}

int	status_neat(t_d *var, t_p philo)
{
	pthread_mutex_lock(&var->lockneat);
	if (philo.nbr_eat >= var->nbr_each_philo_eat)
	{
		pthread_mutex_unlock(&var->lockneat);
		return (1);
	}
	pthread_mutex_unlock(&var->lockneat);
	return (0);
}

void	ft_print_msg_lock(t_d *var, int id, char *str)
{
	if (status_end(var) != 0)
		return ;
	pthread_mutex_lock(&var->lockwrite);
	if (status_dead(var) == 0)
	{
		printf("%li ", ft_return_millisec() - var->begin);
		printf("%i ", id);
		printf("%s\n", str);
	}
	pthread_mutex_unlock(&var->lockwrite);
	return ;
}
