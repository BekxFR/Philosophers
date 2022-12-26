
#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>

typedef struct s_philo
{
	int				pos;
	int				index;
	int				t_die;
	int				t_eat;
	long int		t_n_eat;
	int				need_eat;
	int				t_sleep;
	enum			{EAT,SLEEP,THINK,WAIT,DEAD} status ;
	int				fright;
	int				fleft;
	pthread_t		thread_id;

	struct	s_data	*data;
}	t_p;

typedef struct s_data
{
	long int		begin;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				nbr_of_philo;
	int				dead;
	int				format;
	int				nbr_each_philo_eat;
	pthread_mutex_t	lockdead;
	pthread_mutex_t	lockstatus;
	pthread_mutex_t	lockwrite;
	pthread_mutex_t	lockvalue;
	pthread_mutex_t	lockeatcount;
	pthread_mutex_t	*fork;

	pthread_t	thread_main;

}	t_d;


/* ft_init_var.c */
void	ft_putstr_fd(char *str, int fd);
int		ft_digit_only_arg(char *str);
int		ft_strlen(char *str);
int		ft_init_arg(t_d *var, int argc, char **argv);
int		ft_init_var(t_d *var);
void	ft_destroy_mutex(t_d *var);

#endif
