## Vue d’ensemble

### Choses à savoir si vous souhaitez réussir cet exercice :

    • Des philosophes (un philosophe minimum) sont assis autour d’une table ronde au centre de laquelle se trouve un grand plat de spaghetti.
    • Les philosophes sont soit en train de manger, de penser ou de dormir.
    	• Lorsqu’ils mangent, ils ne peuvent pas penser ou dormir.
    	• Lorsqu’ils dorment, ils ne peuvent pas manger ou penser.

### Lorsqu’ils pensent, ils ne peuvent pas manger ou dormir.

    • Il y a également des fourchettes sur la table. Il y a autant de fourchettes que
    de philosophes.
    • Puisque servir et manger des spaghetti à l’aide d’une seule fourchette n’est pas
    chose facile, un philosophe prend la fourchette sur sa gauche et celle sur sa droite,
    soit une fourchette dans chaque main, afin de manger.
    • Quand un philosophe a fini de manger, il repose les fourchettes sur la table et se
    met à dormir. Une fois réveillé, il se remet à penser. La simulation prend fin si un
    philosophe meurt de faim.
    • Chaque philosophe a besoin de manger et ne doit pas mourir de faim.
    • Les philosophes ne communiquent pas entre eux.
    • Les philosophes ne peuvent pas savoir si l’un d’entre eux est sur le point de mourir.
    • Inutile de préciser que les philosophes ne doivent pas mourir !

### Règles communes

Vous devez faire un programme pour la partie obligatoire et, dans le cas où vous choisissez aussi de faire les bonus, un programme pour la partie bonus.

#### Ils doivent respecter les règles suivantes, communes aux deux parties :

    • Les variables globales sont interdites !
    • Chaque programme doit prendre les arguments suivants :
    number_of_philosophers time_to_die time_to_eat time_to_sleep
    [number_of_times_each_philosopher_must_eat]
    ◦ number_of_philosophers : Le nombre de philosophes, mais aussi le nombre
    de fourchettes.
    ◦ time_to_die (en millisecondes) : Si un philosophe n’a pas commencé à manger
    time_to_die millisecondes après le début de son précédent repas ou depuis le
    début de la simulation, il meurt.
    ◦ time_to_eat (en millisecondes) : Le temps qu’un philosophe prend à manger.
    Pendant ce temps, un philosophe doit garder ses deux fourchettes.
    ◦ time_to_sleep (en millisecondes) : Le temps qu’un philosophe passe à dormir.
    ◦ number_of_times_each_philosopher_must_eat (argument optionnel) : Si tous
    les philosophes ont mangé au moins number_of_times_each_philosopher_must_eat
    fois, la simulation prend fin. Si cet argument n’est pas spécifié, alors la simulation prend fin à la mort d’un philosophe.
    • Chaque philosophe se voit assigner un numéro entre 1 et number_of_philosophers.
    • Le philosophe numéro 1 est assis à côté du philosophe numéro number_of_philosophers.

Les autres suivent cette logique : philosophe numéro N est assis entre philosophe
numéro N - 1 et philosophe numéro N + 1.

#### Concernant les logs de votre programme :

    • Tout changement d’état d’un philosophe doit être formatté comme suit :
    ◦ timestamp_in_ms X has taken a fork
    ◦ timestamp_in_ms X is eating
    ◦ timestamp_in_ms X is sleeping
    ◦ timestamp_in_ms X is thinking
    ◦ timestamp_in_ms X died
    Remplacez timestamp_in_ms par le timestamp actuel en millisecondes
    et X par le numéro du philosophe.
    • Tout message affiché ne doit pas être mélangé avec un autre message.
    • Il ne doit pas y avoir plus de 10 ms entre la mort d’un philosophe et l’affichage du
    message annonçant sa mort.
    • Encore une fois, les philosophes doivent éviter de mourir.

Votre programme ne doit pas avoir de data race.

### Partie obligatoire

Nom du programme: philo

    - Fichiers de rendu Makefile, *.h, *.c, dans un dossier philo/
    	Makefile NAME, all, clean, fclean, re

    - Arguments :
    number_of_philosophers
    time_to_die time_to_eat
    time_to_sleep
    [number_of_times_each_philosopher_must_eat]

    - Fonctions externes autorisées :
    	memset, printf, malloc, free, write,
    	usleep, gettimeofday, pthread_create,
    	pthread_detach, pthread_join, pthread_mutex_init,
    	pthread_mutex_destroy, pthread_mutex_lock,
    	pthread_mutex_unlock

    Libft autorisée Non

### Description Philosophers avec des threads et des mutex

#### Les règles spécifiques à la partie obligatoire sont :

    • Chaque philosophe doit être représenté par un thread.
    • Une fourchette est placée entre chaque paire de philosophes. Cela signifie que, s’il
    y a plusieurs philosophes, chaque philosophe a une fourchette à sa gauche et une à
    sa droite. S’il n’y a qu’un seul philosophe, il n’y aura donc qu’une seule fourchette
    sur la table.
    • Afin d’empêcher les philosophes de dupliquer les fourchettes, vous devez protéger
    leur état en mémoire avec un mutex pour chacune d’entre elle.
