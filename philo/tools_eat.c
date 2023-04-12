/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 18:34:40 by chillion          #+#    #+#             */
/*   Updated: 2023/01/10 18:34:43 by chillion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_eat_left(t_p *philo, t_d *var)
{
	if (status_dead(var) != 0 || status_end(var) != 0)
		return (1);
	pthread_mutex_lock(&(var->fork[philo->fleft]));
	pthread_mutex_lock(&(var->fork[philo->fright]));
	ft_print_msg_lock(var, philo->index, "has taken a fork");
	ft_print_msg_lock(var, philo->index, "has taken a fork");
	ft_print_msg_lock(var, philo->index, "is eating");
	pthread_mutex_lock(&var->lockneat);
	philo->nbr_eat++;
	pthread_mutex_unlock(&var->lockneat);
	pthread_mutex_lock(&var->lockeat);
	philo->t_n_eat = (ft_return_millisec() + var->t_die);
	pthread_mutex_unlock(&var->lockeat);
	ft_usleep_milli(philo, var->t_eat);
	pthread_mutex_unlock(&(var->fork[philo->fright]));
	pthread_mutex_unlock(&(var->fork[philo->fleft]));
	return (0);
}

int	ft_eat_right(t_p *philo, t_d *var)
{
	if (status_dead(var) != 0 || status_end(var) != 0)
		return (1);
	pthread_mutex_lock(&var->fork[philo->fright]);
	pthread_mutex_lock(&var->fork[philo->fleft]);
	ft_print_msg_lock(var, philo->index, "has taken a fork");
	ft_print_msg_lock(var, philo->index, "has taken a fork");
	ft_print_msg_lock(var, philo->index, "is eating");
	pthread_mutex_lock(&var->lockneat);
	philo->nbr_eat++;
	pthread_mutex_unlock(&var->lockneat);
	pthread_mutex_lock(&var->lockeat);
	philo->t_n_eat = (ft_return_millisec() + var->t_die);
	pthread_mutex_unlock(&var->lockeat);
	ft_usleep_milli(philo, var->t_eat);
	pthread_mutex_unlock(&var->fork[philo->fleft]);
	pthread_mutex_unlock(&var->fork[philo->fright]);
	return (0);
}
