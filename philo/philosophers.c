/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 11:59:07 by chillion          #+#    #+#             */
/*   Updated: 2022/12/26 17:54:23 by chillion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



/*
Arguments :
	number_of_philosophers // NBR PHILO & FOURCHETTE // Entre 1 ET number_of_philosophers
	time_to_die // (en millisecondes) DEBUT SIMU OU AFTER EAT
	time_to_eat // (en millisecondes) TIME TO EAT (KEEP 2 FOURCHETTES)
	time_to_sleep // (en millisecondes) TIME TO SLEEP
	[number_of_times_each_philosopher_must_eat] // (optionnel)
			IF ALL PHILO EATS == NOTEPME == END || END == DIE OF 1 PHILO

			   N-1		   N-1
		PHILO 1 -> PHILO 2 -> PHILO3
			   N+1		   N+1ork_
	  		   F1		   F2		F3
*/

/*
Fonctions externes autorisées :
	printf
	malloc
	free
	write
	
	memset
	usleep

	gettimeofday

	pthread_create // create thread
	pthread_join // join thread
	pthread_detach // 
	
	INIT VERROU ET LOCK
	pthread_mutex_init // mise en veille si tentative pendant mutex -- verrou liberable uniquement par le thread qui l'a init
	int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr)
	Crée un mutex, référencé par mutex, avec les attributs spécifiés par attr.
	Si attr est NULL, l'attribut mutex par défaut (NONRCURSIVE) est utilisé.
	Valeur renvoyée
	En cas de succès, pthread_mutex_init() renvoie 0 et l'état du mutex devient initialisé et déverrouillé.
	En cas d'échec, pthread_mutex_init() renvoie -1.
		
	pthread_mutex_lock // lock 1 ou n fois -- need unlock == lock
	int pthread_mutex_lock(pthread_mutex_t *mutex)
	Verrouille un objet mutex, qui identifie un mutex.
	Si le mutex est déjà verrouillé par un autre thread, le thread attend que le mutex soit disponible.
	Le thread qui a verrouillé un mutex devient son propriétaire actuel et le reste
	jusqu'à ce que le même thread l'ait déverrouillé.
	Lorsque le mutex a l'attribut récursif, l'utilisation du verrou peut être différente.
	Lorsque ce type de mutex est verrouillé plusieurs fois par le même thread,
	un décompte est incrémenté et aucun thread en attente n'est publié.
	Le thread propriétaire doit appeler pthread_mutex_unlock() le même nombre de fois
	pour décrémenter le compte à zéro.
	Valeur renvoyée
	En cas de succès, pthread_mutex_lock() renvoie 0.
	En cas d'échec, pthread_mutex_lock() renvoie -1.

	UNLOCK ET DESTROY LOCKvoidu
	int pthread_mutex_unlock(pthread_mutex_t *mutex)
	Libère un objet mutex.
	Si un ou plusieurs threads attendent pour verrouiller le mutex,
	pthread_mutex_unlock() provoque le retour d'un de ces threads depuis
	pthread_mutex_lock() avec l'objet mutex acquis.
	Si aucun thread n'attend le mutex, le mutex se déverrouille sans propriétaire actuel.
	Lorsque le mutex a l'attribut récursif, l'utilisation du verrou peut être différente.
	Lorsque ce type de mutex est verrouillé plusieurs fois par le même thread
	alors déverrouiller décrémente le nombre et aucun thread en attente n'est posté
	pour continuer à fonctionner avec le verrou. Si le compte est décrémenté à zéro,
	alors le mutex est libéré et si un thread l'attend, il est posté.
	Valeur renvoyée	printf("char 0=%c strlen i=%ld\n", str[0], i);

	pthread_mutex_destroy // detruit le verrou pour ne plus pouvoir l'utiliser
	int pthread_mutex_destroy(pthread_mutex_t *mutex)
	Supprime un objet mutex, qui identifie un mutex.
	Les mutex sont utilisés pour protéger les ressources partagées.
	mutex est défini sur une valeur non valide,
	mais peut être typedef struct s_philo
{
	int				pos;
	int				index;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	enum {EAT,SLEEP,THINK,WAIT} status ;
	pthread_mutex_t	fork_right;
	pthread_mutex_t	fork_le	printf("char 0=%c strlen i=%ld\n", str[0], i);


typedef struct s_data
{
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				nbr_of_philo;
	int				dead;
	int				nbr_each_philo_eat;
	pthread_mutex_t	lockeat;
	pthread_mutex_t	locksleep;philo[2].thread_id
}	t_d;ni dormir 
1 philo = 1 fourchette
manger == 2 fourchettes, celle de gauche et de droite == 1 dans chaque main
>> pose les fourchettes >> dormir // manger ==>> dormir
dormir ==>> penser
Si un philo meurt == END

un philo DOIT manger pour ne pas mourrir de faim

un philo NE PEUT PAS communiquer avec un philo PHILO<<NOK>>PHILO
#include "philosophers.h"
un philo ne connait pas l'etat des autres philos

un philo ne doit pas mourrir

philo[2].thread_id.
Encore une fois, les philosophes doivent éviter de mourir.

1 PHILO = 1 THREAD
1 Fourchette = 1 MUTEX	printf("char 0=%c strlen i=%ld\n", str[0], i);

	detach thread philo
*/

#include "philosophers.h"

long int	ft_return_millisec(void)
{
	struct timeval current_time;
	if (gettimeofday(&current_time, 0) == -1)
		return (write(2, "gettimeofday Error\n", 20), -1);
	long int milliseconds = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (milliseconds);
}

// int	ft_eat(t_p *var)
// {
// 	long int	time = ft_return_millisec(var);
// 	long int	eat_time = 0;

// 	(void)var;
// 	(void)time;
// 	(void)eat_time;
// 	sleep(2);
// 	eat_time = (ft_return_millisec(var));
// 	long int dif = (eat_time - time);
// 	printf("Begin eat for eat time = %ld et time eat =%ld et dif=%ld\n", time, eat_time, dif);
// 	// printf("var->t_eat=%d, time=%f et eat_time =%f\n", var->t_eat, time, eat_time);
// 	usleep((var->t_eat * 1000));
// 	pthread_mutex_lock(&var->lockeatcount);
// 	var->nbr_each_philo_eat--;
// 	pthread_mutex_unlock(&var->lockeatcount);
// 	// while (eat_time < var->t_eat)
// 	// {
// 	// 	usleep(var->t_sleep);
// 	// 	eat_time = (ft_return_time(var) - time);
// 	// 	printf("var->t_eat=%d, time=%f et eat_time =%f\n", var->t_eat, time, eat_time);
// 	// 	usleep(var->t_sleep);
// 	// }
// 	return (0);
// }

// void	ft_check_tfork(t_p *var, int k)
// {
// 	int j;

// 	j = 0;
// 	while (j == 0)
// 	{
// 		j = 0;
// 		if (var->nbr_philo > 1)
// 		{
// 			if ((var->i[k][0] - 1) == 0)
// 			{
// 				printf("IN == 0\n");
// 				if (var->tfork[var->i[k][0] - 1][0] == 0 && var->tfork[var->nbr_philo - 1][0] == 0)
// 				{
// 					pthread_mutex_lock(&var->lockeat);
// 					var->tfork[var->i[k][0] - 1][0] = 1;
// 					var->tfork[var->nbr_philo - 1][0] = 1;
// 					ft_eat(var);
// 					var->tfork[var->i[k][0] - 1][0] = 0;
// 					var->tfork[var->nbr_philo - 1][0] = 0;
// 					j = 1;
// 					pthread_mutex_unlock(&var->lockeat);
// 				}
// 			}
// 			else
// 			{
// 				if (var->tfork[var->i[k][0] - 1][0] == 0 && var->tfork[var->i[k][0]][0] == 0)
// 				{
// 					pthread_mutex_lock(&var->lockeat);
// 					var->tfork[var->i[k][0]][0] = 1;
// 					var->tfork[var->i[k][0] - 1][0] = 1;
// 					ft_eat(var);
// 					var->tfork[var->i[k][0]][0] = 0;
// 					var->tfork[var->i[k][0] - 1][0] = 0;
// 					j = 1;
// 					pthread_mutex_unlock(&var->lockeat);
// 				}
// 			}
// 		}
// 	}
// }

// void *ft_philo_life(void *t_data)
// {
// 	// (void)t_data;
// 	int k;
// 	t_d *var = (t_d *)t_data;

// 	pthread_mutex_lock(&var->lockvalue);
// 	k = 0;
// 	printf("IN Thread, thread_id[%d]=%d\n", var->i[k][0], var->i[k][1]);
// 	ft_check_tfork(var, k);
// 	printf("VALUE MUTEX\n");
// 	pthread_mutex_unlock(&var->lockvalue);
// 	// printf("IN Thread, thread_id[%d]=%d\n", var->i[var->cur_philo][0], var->i[var->cur_philo][1]);
// 	// usleep(400);
// 	pthread_mutex_lock(&var->lockwrite);
// 	printf("Philo %d is sleeping\n", var->i[k][0]);
// 	pthread_mutex_unlock(&var->lockwrite);
// 	usleep((var->t_sleep * 1000));
// 	printf("Philo %d is thinking\n", var->i[k][0]);
// 	return NULL;
// }

	// j = 0;
	// while (j == 0)
	// {
	// 	j = 0;
	// 	if (var->nbr_philo > 1)
	// 	{
	// 		if ((var->i[var->cur_philo][0] - 1) == 0)
	// 		{
	// 			if (var->tfork[var->i[var->cur_philo][0] - 1][0] == 0 && var->tfork[var->nbr_philo - 1][0] == 0)
	// 			{
	// 				pthread_mutex_lock(&var->lockeat);
	// 				var->tfork[var->i[var->cur_philo][0] - 1][0] = 1;
	// 				var->tfork[var->nbr_philo - 1][0] = 1;
	// 				ft_eat(var);
	// 				var->tfork[var->i[var->cur_philo][0] - 1][0] = 0;
	// 				var->tfork[var->nbr_p	var->t_die = 0;
	// var->t_eat = 0;
	// var->t_sleep = 0;r->cur_philo][0]][0] = 1;
	// 				var->tfork[var->i[var->cur_philo][0] - 1][0] = 1;
	// 				ft_eat(var);
	// 				var->tfork[var->i[var->cur_philo][0]][0] = 0;
	// 				var->tfork[var->i[var->cur_philo][0] - 1][0] = 0;
	// 				j = 1;
	// 				pthread_mutex_unlock(&var->lockeat);
	// 			}
	// 		}
	// 	}
	// }
	// // usleep(400);
	// pthread_mutex_lock(&var->lockwrite);
	// printf("Philo %d is sleeping\n", var->i[var->cur_philo][0]);
	// pthread_mutex_unlock(&var->lockwrite);
	// usleep((var->t_sleep * 1000));
	// printf("Philo %d is thinking\n", v	var->t_die = 0;
// 	var->t_eat = 0;
// 	var->t_sleep = 0;
// 	return (i);
// }



int	ft_init_tfork(t_d *var)
{
	int	i;
	
	i = 0;
	var->fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * var->nbr_of_philo);
	if (!var->fork)
		return (printf("\n Malloc for mutex init has failed\n"), 1);
	while (i < var->nbr_of_philo)
	{
		if (pthread_mutex_init(&var->fork[i], NULL) != 0)
			return (printf("\n Mutex init has failed\n"), 1);
		i++;
	}
	return (0);
}

void	ft_free_size(void **var, int size)
{
	int	i;

	if (!var)
		return ;
	i = 0;
	while(i < size)
	{
		free(var[i]);
		var[i] = NULL;
		i++;
	}
	free(var);
	var = NULL;
}

int	ft_var_value(t_d *var)
{
	int	i;

	i = ft_init_tfork(var);
	printf("I=%d et var->nbr_philo=%d\n", i ,var->nbr_of_philo);
	if (i != 0)
	{
		write(2, "Exit : cause malloc failure\n", 29);
		return (ft_free_size((void *)var->fork, i), 1);
	}
	var->begin = ft_return_millisec();
	return (0);
}


long int	ft_return_microsec(void)
{
	struct timeval current_time;
	long int microseconds;

	// (void)var;
	if (gettimeofday(&current_time, 0) == -1)
		return (write(2, "gettimeofday Error\n", 20), -1);
	microseconds = ((current_time.tv_sec * 1000) + current_time.tv_usec);
	printf("seconds : %ld et micro seconds : %ld\n", current_time.tv_sec, current_time.tv_usec);
	return (microseconds);
}


int	ft_check_dead(t_p *philo)
{
	if (philo->status == DEAD)
		return (0);
	if (philo->data->format == 1)
	{
		pthread_mutex_lock(&philo->data->lockeatcount);
		if (philo->need_eat <= 0)
		{
			pthread_mutex_lock(&philo->data->lockdead);
			philo->status = DEAD;
			pthread_mutex_unlock(&philo->data->lockdead);
			pthread_mutex_unlock(&philo->data->lockeatcount);
			return (0);
		}
		pthread_mutex_unlock(&philo->data->lockeatcount);
	}
	if (philo->t_n_eat < ft_return_millisec() && philo->status != DEAD)
	{
		pthread_mutex_lock(&philo->data->lockdead);
		philo->data->dead = 1;
		philo->status = DEAD;
		pthread_mutex_unlock(&philo->data->lockdead);
		pthread_mutex_lock(&philo->data->lockwrite);
		printf("%ld P%d died\n", ft_return_millisec(), philo->index);
		pthread_mutex_unlock(&philo->data->lockwrite);
		return (0);
	}
	pthread_mutex_lock(&philo->data->lockdead);
	if (philo->data->dead == 1 && philo->status != DEAD)
	{
		philo->status = DEAD;
		pthread_mutex_unlock(&philo->data->lockdead);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->lockdead);
	return (1);
}

// int	ft_usleep_milli(void *arg)
int	ft_usleep_milli(t_p *philo, int ref)
{
	long int milliseconds;
	// int limit;
	long int interval;
	long int time;

	// limit = *(int *)var;
	time = 0;
	milliseconds = ft_return_millisec();
	// printf("var->data->nbr_each_philo_eat=%d\n", philo->data->nbr_each_philo_eat);
	while (time < ref && philo->t_n_eat > time)
	{
		if (!ft_check_dead(philo))
			return (-1);
		interval = ft_return_millisec();
		time = interval - milliseconds;
	}
	if (philo->data->dead)
		return (-1);
	return (0);
}

int	ft_left_handed(t_p *philo)
{
	pthread_mutex_lock(&philo->data->fork[philo->fleft]);
	if (pthread_mutex_lock(&philo->data->fork[philo->fright]) == -1)
	{
		pthread_mutex_unlock(&philo->data->fork[philo->fleft]);
		return (1);
	}
	pthread_mutex_lock(&philo->data->lockwrite);
	if (!ft_check_dead(philo))
	{
		pthread_mutex_unlock(&philo->data->fork[philo->fright]);
		pthread_mutex_unlock(&philo->data->fork[philo->fleft]);
		return(0);
	}
	printf("%ld P%d has taken fork %d\n", ft_return_millisec(), philo->index, (philo->fleft + 1));
	printf("%ld P%d has taken fork %d\n", ft_return_millisec(), philo->index, (philo->fright + 1));
	printf("%ld P%d is eating\n", ft_return_millisec(), philo->index);
	pthread_mutex_unlock(&philo->data->lockwrite);
	pthread_mutex_lock(&philo->data->lockvalue);
	philo->t_n_eat = (ft_return_millisec() + philo->t_die);
	pthread_mutex_unlock(&philo->data->lockvalue);
	pthread_mutex_lock(&philo->data->lockstatus);
	if (philo->status != DEAD)
		philo->status = EAT;
	pthread_mutex_unlock(&philo->data->lockstatus);
	ft_usleep_milli(philo, philo->t_eat);
	pthread_mutex_unlock(&philo->data->fork[philo->fright]);
	pthread_mutex_unlock(&philo->data->fork[philo->fleft]);
	philo->need_eat--;
	return (0);
}

int	ft_right_handed(t_p *philo)
{
	pthread_mutex_lock(&philo->data->fork[philo->fright]);
	if (pthread_mutex_lock(&philo->data->fork[philo->fleft]) == -1)
	{
		pthread_mutex_unlock(&philo->data->fork[philo->fright]);
		return (1);
	}
	if (!ft_check_dead(philo))
	{
		pthread_mutex_unlock(&philo->data->fork[philo->fleft]);
		pthread_mutex_unlock(&philo->data->fork[philo->fright]);
		// pthread_mutex_lock(&philo->data->lockstatus);
		// philo->status = DEAD;
		// pthread_mutex_unlock(&philo->data->lockstatus);
		return(0);
	}
	pthread_mutex_lock(&philo->data->lockwrite);
	printf("%ld P%d has taken fork %d\n", ft_return_millisec(), philo->index, (philo->fleft + 1));
	printf("%ld P%d has taken fork %d\n", ft_return_millisec(), philo->index, (philo->fright + 1));
	printf("%ld P%d is eating\n", ft_return_millisec(), philo->index);
	pthread_mutex_unlock(&philo->data->lockwrite);
	pthread_mutex_lock(&philo->data->lockvalue);
	philo->t_n_eat = (ft_return_millisec() + philo->t_die);
	pthread_mutex_unlock(&philo->data->lockvalue);
	ft_usleep_milli(philo, philo->t_eat);
	pthread_mutex_unlock(&philo->data->fork[philo->fleft]);
	pthread_mutex_unlock(&philo->data->fork[philo->fright]);
	pthread_mutex_lock(&philo->data->lockstatus);
	if (!ft_check_dead(philo) && philo->status != DEAD)
		philo->status = EAT;
	pthread_mutex_unlock(&philo->data->lockstatus);
	philo->need_eat--;
	return (0);
}

int	ft_eat(t_p *philo)
{
	while (ft_check_dead(philo) && philo->status == WAIT)
	{
		if (philo->pos % 2 == 0)
			ft_left_handed(philo);
		else
			ft_right_handed(philo);
	}
	return (0);
}

int	ft_sleep(t_p *philo)
{
	pthread_mutex_lock(&philo->data->lockstatus);
	if (philo->status == EAT)
	{
		philo->status = SLEEP;
		pthread_mutex_unlock(&philo->data->lockstatus);
		pthread_mutex_lock(&philo->data->lockwrite);
		printf("%ld P%d is sleeping\n", ft_return_millisec(), philo->index);
		pthread_mutex_unlock(&philo->data->lockwrite);
		ft_usleep_milli(philo, philo->t_sleep);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->lockstatus);
	return (0);
}

int	ft_think(t_p *philo)
{
	pthread_mutex_unlock(&philo->data->lockstatus);
	if (philo->status == SLEEP)
	{
		philo->status = THINK;
		pthread_mutex_lock(&philo->data->lockwrite);
		printf("%ld P%d is thinking\n", ft_return_millisec(), philo->index);
		pthread_mutex_unlock(&philo->data->lockwrite);
		ft_usleep_milli(philo, 500);
		return(0);
	}
	pthread_mutex_unlock(&philo->data->lockstatus);
	return (0);
}

void	*ft_main_thread(void *arg)
{
	(void)arg;
	t_p *var;

	var = (t_p *)arg;
	printf("var->t_n_die=%ld\n", var->t_n_eat);
	while(var->status != DEAD && !var->data->dead)
	{
		if (!ft_check_dead(var))
			return (0);
		if (ft_eat(var) == -1)
			return (0);
		if (ft_sleep(var) == -1)
			return (0);
		if (ft_think(var) == -1)
			return (0);
		if (var->status == THINK)
			var->status = WAIT;
	}
	return (0);
}


int	ft_init_mutex(t_d *var)
{
	if (pthread_mutex_init(&var->lockdead, NULL) != 0)
		return (printf("\n mutex init has failed\n"), 1);
	if (pthread_mutex_init(&var->lockeatcount, NULL) != 0)
		return (printf("\n mutex init has failed\n"), 1);
	if (pthread_mutex_init(&var->lockwrite, NULL) != 0)
		return (printf("\n mutex init has failed\n"), 1);
	if (pthread_mutex_init(&var->lockvalue, NULL) != 0)
		return (printf("\n mutex init has failed\n"), 1);
	if (pthread_mutex_init(&var->lockstatus, NULL) != 0)
		return (printf("\n mutex init has failed\n"), 1);
	return (0);
}

void	*ft_philo_launcher(void *arg)
{
	int	i;
	t_p	*philo;
	t_d *data;

	i = 0;
	data = (t_d *)arg;
	philo = (t_p *)malloc(sizeof(t_p) * data->nbr_of_philo);
	while(i < data->nbr_of_philo)
	{
		philo[i].pos = i;
		philo[i].index = (i + 1);
		philo[i].t_die = data->t_die;
		philo[i].t_eat = data->t_eat;
		philo[i].t_n_eat = (data->begin + (long int)data->t_die);
		philo[i].need_eat = data->nbr_each_philo_eat;
		philo[i].t_sleep = data->t_sleep;
		philo[i].status = WAIT;
		philo[i].data = data;
		philo[i].fleft = i;
		if (i == 0)
			philo[i].fright = data->nbr_of_philo - 1;
		else
			philo[i].fright = i - 1;
		if (pthread_create(&philo[i].thread_id, NULL, ft_main_thread, (void *)&philo[i]) != 0)
			return ((void *)1);
		if(i % 2 != 0)
		{
			if (data->nbr_of_philo > 5)
				usleep(400);
			else
				usleep(50);
		}
		i++;
	}
	i = 0;
	while(i < data->nbr_of_philo)
	{
		if (pthread_join(philo[i].thread_id, NULL) != 0)
			return ((void *)2);
		i++;
	}
	return (philo);
}

int	main(int argc, char **argv)
{
	t_d var;
	t_p *philo;
	
	if (argc < 5 || argc > 6)
		return (write(2, "Wrong number of arguments\n", 27), 1);
	if (ft_init_var(&var) || ft_init_arg(&var, argc, argv) \
	|| ft_var_value(&var) || ft_init_mutex((void *)&var))
		return (1);
	printf("Diner begin at = %ld with %d Philosophers\n", var.begin, var.nbr_of_philo);
	if (pthread_create(&var.thread_main, NULL, ft_philo_launcher, (void *)&var) != 0)
		return (1);
	if (pthread_join(var.thread_main, (void *)&philo) != 0)
		return(2);
	ft_destroy_mutex(&var);
	philo = (t_p *)philo;
	free(&philo[0]);
	printf("After Thread\n");
	return (0);
}
