
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
	long int i;

	i = 0;
	while(str[i])
		i++;
	return (i);
}

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
		var->nbr_each_philo_eat = ft_digit_only_arg(argv[5]);
	printf("\nphilo=%d, t_die=%d, t_eat=%d, t_sleep=%d\n",var->nbr_of_philo, var->t_die, var->t_eat, var->t_sleep);
	if (argc == 6)
		printf("nbr_each_philo_eat=%d,\n", var->nbr_each_philo_eat);
	printf("\n");
	return (0);
}

int	ft_init_var(t_d *var)
{
	var->t_die = 0;
	var->t_eat = 0;
	var->t_sleep = 0;
	var->nbr_of_philo = 0;
	var->dead = 0;
	var->nbr_each_philo_eat = 0;
	var->fork = 0;
	return (0);
}

void	ft_destroy_mutex(t_d *var)
{
	int	i;

	i = 0;
	while (i < var->nbr_of_philo)
	{
		pthread_mutex_destroy(&var->fork[i]);
		i++;
	}
	free(var->fork);
	pthread_mutex_destroy(&var->lockeat);
	pthread_mutex_destroy(&var->locksleep);
	pthread_mutex_destroy(&var->lockwrite);
	pthread_mutex_destroy(&var->lockvalue);
}
