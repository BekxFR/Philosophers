/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chillion <chillion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 11:59:07 by chillion          #+#    #+#             */
/*   Updated: 2022/12/22 17:58:09 by chillion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

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
			   N+1		   N+1
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

	UNLOCK ET DESTROY LOCK
	pthread_mutex_unlock // libere le verrou
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
	Valeur renvoyée
	En cas de succès, pthread_mutex_unlock() renvoie 0.
	En cas d'échec, pthread_mutex_unlock() renvoie -1
	
	pthread_mutex_destroy // detruit le verrou pour ne plus pouvoir l'utiliser
	int pthread_mutex_destroy(pthread_mutex_t *mutex)
	Supprime un objet mutex, qui identifie un mutex.
	Les mutex sont utilisés pour protéger les ressources partagées.
	mutex est défini sur une valeur non valide,
	mais peut être réinitialisé à l'aide de pthread_mutex_init().
	Valeur renvoyée
	En cas de succès, pthread_mutex_destroy() renvoie 0.
	En cas d'échec, pthread_mutex_destroy() renvoie -1.
*/

/*
philo >= 1
3 etats : manger, penser et dormir
manger == ni dormir ni penser 
dormir == ni penser ni manger 
penser == ni manger ni dormir 
1 philo = 1 fourchette
manger == 2 fourchettes, celle de gauche et de droite == 1 dans chaque main
>> pose les fourchettes >> dormir // manger ==>> dormir
dormir ==>> penser
Si un philo meurt == END

un philo DOIT manger pour ne pas mourrir de faim

un philo NE PEUT PAS communiquer avec un philo PHILO<<NOK>>PHILO

un philo ne connait pas l'etat des autres philos

un philo ne doit pas mourrir


FORMAT LOG
	CHANGE ETAT :
		◦ timestamp_in_ms X has taken a fork
		◦ timestamp_in_ms X is eating
		◦ timestamp_in_ms X is sleeping
		◦ timestamp_in_ms X is thinking
		◦ timestamp_in_ms X died
	timestamp_in_ms = timestamp && X == PHiLO NBR 
Tout message affiché ne doit pas être mélangé avec un autre message.
10 ms MAX entre mort d’un philo et l’affichage du message annonçant sa mort.
Encore une fois, les philosophes doivent éviter de mourir.

1 PHILO = 1 THREAD
1 Fourchette = 1 MUTEX
*/


/*
mutex fork
fork[philo] = 0
fork[philo - 1] = 0;
usleep(eat); 
fork[philo] = 1
fork[philo - 1] = 1;

before leave or if 1 philo die;
while I < nbrphilo
	detach thread philo
*/

typedef struct s_info
{
	int				nbr;
	int				t_die;
	pthread_t		thread_id;
}	t_i;

typedef struct s_philosophers
{
	int				nbr_philo;
	int				cur_philo;
	int				cur_philo2;
	int				cur_philo_lock;
	int				t_die;
	int				t_eat;
	int				t_sleep;
	int				dead;
	int				nbr_each_philo_eat;
	int				**tfork;
	pthread_mutex_t	lockeat;
	pthread_mutex_t	locksleep;
	pthread_mutex_t	lockwrite;
	pthread_mutex_t	lockvalue;
	pthread_mutex_t	lockeatcount;
	pthread_t		thread_main;
	pthread_t		thread_id[1024];

	t_i				*philo;
	int **i;
}	t_p;

long int	ft_return_millisec(void* var)
{
	(void)var;
	struct timeval current_time;
	if (gettimeofday(&current_time, 0) == -1)
		return (write(2, "gettimeofday Error\n", 20), -1);
	long int milliseconds = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (milliseconds);
}

int	ft_eat(t_p *var)
{
	long int	time = ft_return_millisec(var);
	long int	eat_time = 0;

	(void)var;
	(void)time;
	(void)eat_time;
	sleep(2);
	eat_time = (ft_return_millisec(var));
	long int dif = (eat_time - time);
	printf("Begin eat for eat time = %ld et time eat =%ld et dif=%ld\n", time, eat_time, dif);
	// printf("var->t_eat=%d, time=%f et eat_time =%f\n", var->t_eat, time, eat_time);
	usleep((var->t_eat * 1000));
	pthread_mutex_lock(&var->lockeatcount);
	var->nbr_each_philo_eat--;
	pthread_mutex_unlock(&var->lockeatcount);
	// while (eat_time < var->t_eat)
	// {
	// 	usleep(var->t_sleep);
	// 	eat_time = (ft_return_time(var) - time);
	// 	printf("var->t_eat=%d, time=%f et eat_time =%f\n", var->t_eat, time, eat_time);
	// 	usleep(var->t_sleep);
	// }
	return (0);
}

void	ft_check_tfork(t_p *var, int k)
{
	int j;

	j = 0;
	while (j == 0)
	{
		j = 0;
		if (var->nbr_philo > 1)
		{
			if ((var->i[k][0] - 1) == 0)
			{
				printf("IN == 0\n");
				if (var->tfork[var->i[k][0] - 1][0] == 0 && var->tfork[var->nbr_philo - 1][0] == 0)
				{
					pthread_mutex_lock(&var->lockeat);
					var->tfork[var->i[k][0] - 1][0] = 1;
					var->tfork[var->nbr_philo - 1][0] = 1;
					ft_eat(var);
					var->tfork[var->i[k][0] - 1][0] = 0;
					var->tfork[var->nbr_philo - 1][0] = 0;
					j = 1;
					pthread_mutex_unlock(&var->lockeat);
				}
			}
			else
			{
				if (var->tfork[var->i[k][0] - 1][0] == 0 && var->tfork[var->i[k][0]][0] == 0)
				{
					pthread_mutex_lock(&var->lockeat);
					var->tfork[var->i[k][0]][0] = 1;
					var->tfork[var->i[k][0] - 1][0] = 1;
					ft_eat(var);
					var->tfork[var->i[k][0]][0] = 0;
					var->tfork[var->i[k][0] - 1][0] = 0;
					j = 1;
					pthread_mutex_unlock(&var->lockeat);
				}
			}
		}
	}
}

void *ft_philo_life(void *t_data)
{
	// (void)t_data;
	int k;
	t_p *var = (t_p *)t_data;

	pthread_mutex_lock(&var->lockvalue);
	k = 0;
	printf("IN Thread, thread_id[%d]=%d\n", var->i[k][0], var->i[k][1]);
	ft_check_tfork(var, k);
	printf("VALUE MUTEX\n");
	pthread_mutex_unlock(&var->lockvalue);
	// printf("IN Thread, thread_id[%d]=%d\n", var->i[var->cur_philo][0], var->i[var->cur_philo][1]);
	// usleep(400);
	pthread_mutex_lock(&var->lockwrite);
	printf("Philo %d is sleeping\n", var->i[k][0]);
	pthread_mutex_unlock(&var->lockwrite);
	usleep((var->t_sleep * 1000));
	printf("Philo %d is thinking\n", var->i[k][0]);
	return NULL;
}

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
	// 				var->tfork[var->nbr_philo - 1][0] = 0;
	// 				j = 1;
	// 				pthread_mutex_unlock(&var->lockeat);
	// 			}
	// 		}
	// 		else
	// 		{
	// 			if (var->tfork[var->i[var->cur_philo][0] - 1][0] == 0 && var->tfork[var->i[var->cur_philo][0]][0] == 0)
	// 			{
	// 				pthread_mutex_lock(&var->lockeat);
	// 				var->tfork[var->i[var->cur_philo][0]][0] = 1;
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
	// printf("Philo %d is thinking\n", var->i[var->cur_philo][0]);
	// return NULL;


int	ft_ptrlen(char *str)
{
	int i = 0;
	if (!str)
		return (i);
	while(str[i])
	{
		i++;
	}
	return (i);
}

int	ft_digit_only_arg(char *str)
{
	int	i = 0;
	long int nbr = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if (!(str[i] >= 0 && str[i] <= 9))
			return (-1);
		nbr = (nbr * 10) + (str[i] + 48);
		i++;
	}
	return (nbr);
}

void	ft_init_arg(t_p *var, int argc, char **argv)
{
	if (argv[1] && ft_digit_only_arg(argv[1]))
		var->nbr_philo = atoi(argv[1]);
	else(write(2, "Error With Arguments\n", 22), exit(1));
	if (argv[2] && ft_digit_only_arg(argv[2]))
		var->t_die = atoi(argv[2]);
	else(write(2, "Error With Arguments\n", 22), exit(1));
	if (argv[3] && ft_digit_only_arg(argv[3]))
		var->t_eat = atoi(argv[3]);
	else(write(2, "Error With Arguments\n", 22), exit(1));
	if (argv[4] && ft_digit_only_arg(argv[4]))
		var->t_sleep = atoi(argv[4]);
	else(write(2, "Format of optional argument is not correct\n", 44), exit(1));
	if (argc == 6)
	{
		if (argv[5] && ft_ptrlen(argv[5]))
			var->nbr_each_philo_eat = atoi(argv[5]);
		else(write(2, "Error With Arguments\n", 22), exit(1));
	}
	printf("\nphilo=%d, t_die=%d, t_eat=%d, t_sleep=%d\n",var->nbr_philo, var->t_die, var->t_eat, var->t_sleep);
	if (argc == 6)
		printf("nbr_each_philo_eat=%d,\n", var->nbr_each_philo_eat);
	printf("\n");
}

void	ft_init_var(t_p *var)
{
	var->nbr_philo = 0;
	var->cur_philo = 0;
	var->cur_philo2 = 0;
	var->cur_philo_lock = 0;
	var->t_die = 0;
	var->t_eat = 0;
	var->t_sleep = 0;
	var->dead = 0;
	var->nbr_each_philo_eat = 0;
	var->tfork = 0;
	var->i = 0;
}

int	ft_init_tfork(t_p *var)
{
	int	i;
	
	i = 0;
	var->tfork = (int **)malloc(sizeof(int *) * (var->nbr_philo + 1));
	if (!var->tfork)
		return (0);
	var->tfork[var->nbr_philo] = 0;
	while (i < var->nbr_philo)
	{
		var->tfork[i] = (int *)malloc(sizeof(int) * (1));
		if (!var->tfork)
			return (i);
		var->tfork[i][0] = 0;
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

void	ft_var_value(t_p *var)
{
	int	i;

	var->nbr_each_philo_eat = var->nbr_each_philo_eat * var->nbr_philo;
	i = ft_init_tfork(var);
	printf("I=%d et var->nbr_philo=%d\n", i ,var->nbr_philo);
	if (i != 0)
	{
		write(2, "Exit : cause malloc failure\n", 29);
		return (ft_free_size((void *)var->tfork, i), exit(1));
	}
}


long int	ft_return_microsec(void* var)
{
	struct timeval current_time;
	long int microseconds;

	(void)var;
	if (gettimeofday(&current_time, 0) == -1)
		return (write(2, "gettimeofday Error\n", 20), -1);
	microseconds = ((current_time.tv_sec * 1000) + current_time.tv_usec);
	printf("seconds : %ld et micro seconds : %ld\n", current_time.tv_sec, current_time.tv_usec);
	return (microseconds);
}

int	ft_usleep_milli(void *var)
{
	long int milliseconds;
	int limit;
	long int interval;
	long int time;

	(void)var;
	limit = *(int *)var;
	time = 0;
	milliseconds = ft_return_millisec(var);
	while((time) < limit)
	{
		interval = ft_return_millisec(var);
		time = interval - milliseconds;
	}
	return (0);
}



void	*ft_main_thread(void *arg)
{
	(void)arg;
	t_p *var;

	var = (t_p *)arg;
	long int time = ft_return_millisec(var);
	printf("time =%ld\n", time);
	sleep(3);
	printf("time =%ld\n", ft_return_millisec(var));
	usleep(3000000);
	printf("time =%ld\n", ft_return_millisec(var));
	ft_usleep_milli((void *)&var->t_die);
	printf("time =%ld\n", ft_return_millisec(var));
	ft_usleep_milli((void *)&var->t_die);
	printf("time =%ld\n", ft_return_millisec(var));
	ft_usleep_milli((void *)&var->t_die);
	printf("time =%ld\n", ft_return_millisec(var));
	ft_usleep_milli((void *)&var->t_die);
	printf("time =%ld\n", ft_return_millisec(var));
	ft_usleep_milli((void *)&var->t_die);
	printf("time =%ld\n", ft_return_millisec(var));
	ft_usleep_milli((void *)&var->t_die);
	printf("time =%ld\n", ft_return_millisec(var));



	
/*
	int	j;
	t_p *var;

	var = (t_p *)arg;
	j = 0;
	while(j < var->nbr_philo)
	{
		var->i[j] = (int *)malloc(sizeof(long int) * (5));
		var->i[j][0] = (j + 1);
		var->i[j][1] = 0;
		var->i[j][2] = 0;
		var->i[j][3] = var->t_die;
		var->i[j][4] = 0;
		j++;
	}
	while (var->cur_philo < var->nbr_philo)
	{
		if (pthread_create(&var->thread_id[var->cur_philo], NULL, ft_philo_life, (void *)&var) != 0)
			return ((void *)&var->cur_philo);
		var->i[var->cur_philo][1] = (var->thread_id[var->cur_philo]);
		printf("Before Thread, thread_id[%d]=%d\n", var->i[var->cur_philo][0], var->i[var->cur_philo][1]);
		// printf("Before Thread, thread_id[%d]=%ld\n", (var->cur_philo + 1), var->thread_id[(var->cur_philo)]);
		usleep(10000);
		var->cur_philo++;
	}
	var->cur_philo2 = 0;
	usleep(1000);
	while (var->cur_philo2 < var->nbr_philo)
	{
		// sleep(5);
		if (pthread_join(var->thread_id[var->cur_philo2], NULL) != 0)
		{
			j = var->cur_philo + var->cur_philo2;
			return((void *)&j);
		}
		usleep(1000);
		var->cur_philo2++;
	}
	// if(1 PHILO DIE OR ALL PHILO EAT)
	//	pthread_detach(var.thread_main);
*/
	return (0);
}

void	ft_test(void *arg)
{
	long int	time;
	long int	time_bdie;
	t_p *var;
	
	var = (t_p *)arg;
	time = ft_return_millisec(var);
	time_bdie = ft_return_millisec(var) + var->t_die;
	while(time < time_bdie)
	{
		if (var->nbr_philo > 1)
		{
			if(var->tfork[])
		}
	}
}

int	main(int argc, char **argv)
{
	t_p var;
	
	if (argc < 5 || argc > 6)
		return 0;
	ft_init_var(&var);
	write(2, "TEST\n", 6);
	ft_init_arg(&var, argc, argv);
	ft_var_value(&var);
	if (pthread_mutex_init(&var.lockeat, NULL) != 0)
		return (printf("\n mutex init has failed\n"), 1);
	if (pthread_mutex_init(&var.locksleep, NULL) != 0)
		return (printf("\n mutex init has failed\n"), 1);
	if (pthread_mutex_init(&var.lockwrite, NULL) != 0)
		return (printf("\n mutex init has failed\n"), 1);
	if (pthread_mutex_init(&var.lockvalue, NULL) != 0)
		return (printf("\n mutex init has failed\n"), 1);
	
	var.i = (int **)malloc(sizeof(int *) * (var.nbr_philo + 1));
	if (!var.i)
		return (0);
	var.i[var.nbr_philo] = 0;
	if (pthread_create(&var.thread_main, NULL, ft_main_thread, (void *)&var) != 0)
		return (1);
	if (pthread_join(var.thread_main, NULL) != 0)
		return(2);
	// usleep(1000);
	pthread_mutex_destroy(&var.lockeat);
	pthread_mutex_destroy(&var.locksleep);
	pthread_mutex_destroy(&var.lockwrite);
	pthread_mutex_destroy(&var.lockvalue);
	printf("After Thread\n");
	return 0;
}
