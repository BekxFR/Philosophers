/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_mutex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 11:09:49 by chillion          #+#    #+#             */
/*   Updated: 2023/01/11 12:09:36 by chillion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_init_mutex(t_d *var)
{
	int	i;

	i = 0;
	while (i < var->nbr_of_philo)
	{
		if (pthread_mutex_init(&var->fork[i], NULL) != 0)
			return (printf("\n Mutex init has failed\n"), 1);
		i++;
	}
	if (pthread_mutex_init(&var->lockdead, NULL) != 0)
		return (printf("\n mutex init has failed\n"), 1);
	if (pthread_mutex_init(&var->lockneat, NULL) != 0)
		return (printf("\n mutex init has failed\n"), 1);
	if (pthread_mutex_init(&var->lockeat, NULL) != 0)
		return (printf("\n mutex init has failed\n"), 1);
	if (pthread_mutex_init(&var->lockdie, NULL) != 0)
		return (printf("\n mutex init has failed\n"), 1);
	if (pthread_mutex_init(&var->lockstatus, NULL) != 0)
		return (printf("\n mutex init has failed\n"), 1);
	if (pthread_mutex_init(&var->lockwrite, NULL) != 0)
		return (printf("\n mutex init has failed\n"), 1);
	if (pthread_mutex_init(&var->lockvalue, NULL) != 0)
		return (printf("\n mutex init has failed\n"), 1);
	return (0);
}

void	ft_destroy_mutex(t_d *var)
{
	pthread_mutex_destroy(&var->lockeat);
	pthread_mutex_destroy(&var->lockdead);
	pthread_mutex_destroy(&var->lockneat);
	pthread_mutex_destroy(&var->lockdie);
	pthread_mutex_destroy(&var->lockstatus);
	pthread_mutex_destroy(&var->lockwrite);
	pthread_mutex_destroy(&var->lockvalue);
}

void	ft_finish(t_d *var, t_p *philo)
{
	int	i;

	i = 0;
	while (i < var->nbr_of_philo)
	{
		pthread_join(philo[i].thread_id, NULL);
		i++;
	}
	i = 0;
	while (i < var->nbr_of_philo)
	{
		pthread_mutex_destroy(&var->fork[i]);
		i++;
	}
	ft_destroy_mutex(var);
}

void	status_die_changer(t_d *var, int i)
{
	ft_print_msg_lock(var, (i + 1), "died");
	pthread_mutex_lock(&(var->lockdead));
	var->dead = 1;
	pthread_mutex_unlock(&(var->lockdead));
}

void	status_checker(t_d *var, t_p *philo)
{
	int	i;

	while (status_end(var) == 0)
	{
		i = 0;
		while (i < var->nbr_of_philo && status_dead(var) == 0)
		{
			usleep(100);
			if (status_eat(&philo[i], ft_return_millisec()) != 0)
				status_die_changer(var, i);
			i++;
		}
		if (status_dead(var) != 0)
			break ;
		i = 0;
		while (i < var->nbr_of_philo && status_neat(var, philo[i]))
			i++;
		if (i == var->nbr_of_philo && var->format == 1)
		{
			pthread_mutex_lock(&var->lockstatus);
			var->status = 1;
			pthread_mutex_unlock(&var->lockstatus);
		}
	}
}
