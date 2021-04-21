#include <stddef.h>
#include <stdio.h>
#include "version.h"

struct commit {
	unsigned long id;
	struct version version;
	char *comment;
	struct commit *next;
	struct commit *prev;
};

struct commit* commit_of(struct version *version) {
    return (struct commit *)((void *)version - (void *) &((struct commit *)0)->version);
}

int main(int argc, char const *argv[])
{
    struct version v = {.major = 3,
			    .minor = 5,
			    .flags = 0};
	struct commit c = {.id = 1,
        .version = v,
        .comment = "comment",
        .next = &c,
        .prev = &c};
    
	printf("@ de id : %ld \n", (long int)&c.id);
    printf("@ de version : %ld \n", (long int)&c.version);
    printf("L'offset de 'version' vaut %lu.\n",
            (unsigned long) offsetof(struct commit, version));
    printf("@ de comment : %ld \n", (long int)&c.comment);
    printf("@ de next : %ld \n", (long int)&c.next);
    printf("@ de prev : %ld \n", (long int)&c.prev);
    printf("Taille du struct : %ld \n", sizeof(c));
    printf("Test commit_of : %s \n", commit_of(&v)->version.major == 3 ? "Valide":"invalide");
    

	return 0;
}

