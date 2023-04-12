/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 10:55:47 by chillion          #+#    #+#             */
/*   Updated: 2023/01/10 18:41:06 by chillion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <limits.h>
# include <sys/time.h>

typedef struct s_philo
{
	int				pos;
	int				index;
	int				t_die;
	int				t_eat;
	int				nbr_eat;
	long int		t_n_eat;
	int				need_eat;
	int				t_sleep;
	int				fright;
	int				fleft;
	pthread_t		thread_id;
	struct s_data	*data;
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
	int				status;
	int				nbr_each_philo_eat;
	pthread_mutex_t	lockneat;
	pthread_mutex_t	lockdead;
	pthread_mutex_t	lockdie;
	pthread_mutex_t	lockstatus;
	pthread_mutex_t	lockwrite;
	pthread_mutex_t	lockvalue;
	pthread_mutex_t	lockeat;
	pthread_mutex_t	fork[300];
	t_p				philo[300];
	pthread_t		thread_main;

}	t_d;

/* philosophers.c */
int			thread_checker(t_p *philo, t_d *var);
void		*ft_main_thread(void *arg);
int			right_diner(t_p *philo, t_d *var);
void		*ft_philo_launcher(t_d *var);

/* tools_eat.c */
int			ft_eat_left(t_p *philo, t_d *var);
int			ft_eat_right(t_p *philo, t_d *var);

/* tools_init.c */
void		ft_putstr_fd(char *str, int fd);
int			ft_digit_only_arg(char *str);
int			ft_strlen(char *str);
void		main_thread_wait(t_p *philo);
int			main_thread_tools(t_d *var, t_p *philo);

/* tools_init_time.c */
int			ft_init_arg(t_d *var, int argc, char **argv);
int			ft_init_var(t_d *var);
int			ft_init_philo(t_d *var);
int			ft_usleep_milli(t_p *philo, int ref);
long int	ft_return_millisec(void);

/* tools_lock.c */
int			status_dead(t_d *var);
int			status_eat(t_p *philo, long int time);
int			status_end(t_d *var);
int			status_neat(t_d *var, t_p philo);
void		ft_print_msg_lock(t_d *var, int id, char *str);

/* tools_mutex.c */
int			ft_init_mutex(t_d *var);
void		ft_destroy_mutex(t_d *var);
void		ft_finish(t_d *var, t_p *philo);
void		status_die_changer(t_d *var, int i);
void		status_checker(t_d *var, t_p *philo);

#endif
