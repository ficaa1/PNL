#include<stdlib.h>
#include<stdio.h>

#include"commit.h"

static int uniqueId=0;

/**
  * new_commit - alloue et initialise une structure commit correspondant aux
  *              parametres
  *
  * @major: numero de version majeure
  * @minor: numero de version mineure
  * @comment: pointeur vers une chaine de caracteres contenant un commentaire
  *
  * @return: retourne un pointeur vers la structure allouee et initialisee
  */
struct commit *new_commit(unsigned short major, unsigned long minor,
			  char *comment)
{
  struct commit *new_commit = malloc(sizeof(struct commit));
  (new_commit->version).major = major;
  (new_commit->version).minor = minor;
  new_commit->comment = comment;
  new_commit->id = uniqueId++;
  return new_commit;
}

/**
  * insert_commit - insere sans le modifier un commit dans la liste doublement
  *                 chainee
  *
  * @from: commit qui deviendra le predecesseur du commit insere
  * @new: commit a inserer - seuls ses champs next et prev seront modifies
  *
  * @return: retourne un pointeur vers la structure inseree
  */
static struct commit *insert_commit(struct commit *from, struct commit *new)
{
	/* TODO : Exercice 3 - Question 3 */
	new->next = from->next;
  new->prev = from;
	if (from->next) from->next->prev = new;
	from->next = new;
	return new;
}

/**
  * add_minor_commit - genere et insere un commit correspondant a une version
  *                    mineure
  *
  * @from: commit qui deviendra le predecesseur du commit insere
  * @comment: commentaire du commit
  *
  * @return: retourne un pointeur vers la structure inseree
  */
struct commit *add_minor_commit(struct commit *from, char *comment)
{
	/* TODO : Exercice 3 - Question 3 */
	struct commit *minor_comm = new_commit(from->version.major, from->version.minor + 1, comment);
  insert_commit(from, minor_comm);
  return minor_comm;
}

/**
	* add_major_commit - genere et insere un commit correspondant a une version
  *                    majeure
  *
  * @from: commit qui deviendra le predecesseur du commit insere
  * @comment: commentaire du commit
  *
  * @return: retourne un pointeur vers la structure inseree
  */
struct commit *add_major_commit(struct commit *from, char *comment)
{
	/* TODO : Exercice 3 - Question 3 */
	struct commit *major_comm = new_commit(from->version.major +1, 0, comment);
  insert_commit(from,major_comm);
  return major_comm;
}

/**
  * del_commit - extrait le commit de l'historique
  *
  * @victim: commit qui sera sorti de la liste doublement chainee
  *
  * @return: retourne un pointeur vers la structure extraite
  */
struct list_head *del_commit(struct list_head *victim)
{
	/* TODO : Exercice 3 - Question 5 */
  list_del(victim)
	return NULL;
}

/**
  * display_commit - affiche un commit : "2:  0-2 (stable) 'Work 2'"
  *
  * @c: commit qui sera affiche
  */
void display_commit(struct commit *c)
{
	/* TODO : Exercice 3 - Question 4 */
printf("%lu:\t%hu.%lu (%s)\t'%s'\n", c->id, c->version.major, c->version.minor,
	        (c->version.minor & 1 ? "unstable" : "stable"),
	        (c->comment ? c->comment : ""));
}

/**
  * commitOf - retourne le commit qui contient la version passee en parametre
  *
  * @version: pointeur vers la structure version dont on recherche le commit
  *
  * @return: un pointeur vers la structure commit qui contient 'version'
  *
  * Note:      cette fonction continue de fonctionner meme si l'on modifie
  *            l'ordre et le nombre des champs de la structure commit.
  */
struct commit *commitOf(struct version *version)
{
  return (struct commit *)((void *)version - (void *) &((struct commit *)0)->version);
}
