#include<stdlib.h>
#include<stdio.h>

#include"history.h"

/**
  * new_history - alloue, initialise et retourne un historique.
  *
  * @name: nom de l'historique
  */
struct history *new_history(char *name)
{
	struct history *history = malloc(sizeof(struct history));
  struct commit *teteFantome = new_commit(0, 0, NULL);
  history->name = name;
  history->commit_count = 1;
  history->commit_list = teteFantome;
  return history;
}

/**
  * last_commit - retourne l'adresse du dernier commit de l'historique.
  *
  * @h: pointeur vers l'historique
  */
struct commit *last_commit(struct history *h)
{
	struct commit *c = h->commit_list;
  while(c->next) c = c->next;
  return c;
}

/**
  * display_history - affiche tout l'historique, i.e. l'ensemble des commits de
  *                   la liste
  *
  * @h: pointeur vers l'historique a afficher
  */
void display_history(struct history *h)
{
	/* TODO : Exercice 3 - Question 2 */
  struct commit *commit = h->commit_list->next;
  printf("Historique de %s \n",h->name);
  while(commit){
    display_commit(commit);
    commit = commit->next;
  }
  printf("\n");
}

/**
  * infos - affiche le commit qui a pour numero de version <major>-<minor> ou
  *         'Not here !!!' s'il n'y a pas de commit correspondant.
  *
  * @major: major du commit affiche
  * @minor: minor du commit affiche
  */
void infos(struct history *h, int major, unsigned long minor)
{
	/* TODO : Exercice 3 - Question 2 */
  struct commit *commit = h->commit_list;
  while(commit){
    if (commit->version.major == (short unsigned int)major && commit->version.minor == minor) {
	        display_commit(commit);
	        return;
	    }
  commit = commit->next;
  }

  printf("Not here !!!");
}
