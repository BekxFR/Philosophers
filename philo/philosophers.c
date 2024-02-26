/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 11:59:07 by chillion          #+#    #+#             */
/*   Updated: 2023/01/10 18:44:35 by chillion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	thread_checker(t_p *philo, t_d *var)
{
	if (status_dead(var) != 0)
		return (1);
	if (status_eat(philo, ft_return_millisec()) != 0)
		return (1);
	if (status_end(var) != 0)
		return (1);
	if (var->format == 1 && philo->nbr_eat >= var->nbr_each_philo_eat)
		return (1);
	return (0);
}

void	*ft_main_thread(void *arg)
{
	t_p	*philo;
	t_d	*var;

	philo = (t_p *)arg;
	var = philo->data;
	if (main_thread_tools(var, philo) == 1)
		return ((int *)1);
	while (1)
	{
		if (thread_checker(philo, var))
			return ((int *)1);
		if ((philo->index % 2) != 0)
			ft_eat_left(philo, var);
		else
			ft_eat_right(philo, var);
		if (thread_checker(philo, var))
			return ((int *)1);
		ft_print_msg_lock(var, philo->index, "is sleeping");
		ft_usleep_milli(philo, (philo->t_sleep));
		ft_print_msg_lock(var, philo->index, "is thinking");
		if (var->nbr_of_philo == philo->index && (var->nbr_of_philo % 2) != 0)
			ft_usleep_milli(philo, (var->t_eat));
		else if ((var->nbr_of_philo % 2) != 0)
			ft_usleep_milli(philo, (var->t_eat / 2));
	}
}

int	right_diner(t_p *philo, t_d *var)
{
	int	i;

	i = 0;
	while (i < var->nbr_of_philo)
	{
		if ((i % 2) != 0)
		{
			if (pthread_create((&philo[i].thread_id), \
			NULL, ft_main_thread, &(philo[i])) != 0)
				return (1);
		}
		i++;
	}
	return (0);
}

void	*ft_philo_launcher(t_d *var)
{
	t_p	*philo;
	int	i;

	philo = var->philo;
	var->begin = ft_return_millisec();
	i = 0;
	while (i < var->nbr_of_philo)
	{
		if ((i % 2) == 0)
		{
			if (pthread_create((&philo[i].thread_id), \
			NULL, ft_main_thread, &(philo[i])) != 0)
				return ((int *)1);
		}
		i++;
	}
	usleep((var->t_eat / 2) * 1000);
	if (right_diner(philo, var))
		return ((int *)1);
	status_checker(var, var->philo);
	ft_finish(var, var->philo);
	return (philo);
}

int	main(int argc, char **argv)
{
	t_d	var;

	if (argc < 5 || argc > 6)
		return (write(2, "Wrong number of arguments\n", 27), 1);
	if (ft_init_var(&var) || ft_init_arg(&var, argc, argv) \
	|| ft_init_philo(&var) || ft_init_mutex((void *)&var))
		return (1);
	ft_philo_launcher(&var);
	return (0);
}
