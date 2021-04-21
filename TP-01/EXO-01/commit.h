#ifndef COMMIT_H
#define COMMIT_H

#include"version.h"
#include"list.h"

struct commit;

struct commit {
	unsigned long id;
	struct version version;
	char *comment;
	struct list_head *next;
	struct list_head *prev;
};

struct list_head *new_commit(unsigned short major, unsigned long minor,
			  char *comment);

struct list_head *add_minor_commit(struct commit *from, char *comment);

struct list_head *add_major_commit(struct commit *from, char *comment);

struct list_head *del_commit(struct list_head *victim);

void display_commit(struct commit *from);

#endif
