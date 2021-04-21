.PHONY : all clean check
.ONESHELL:

CHECK_PATCH=./checkpatch.pl --no-tree

all : testVersion testOffset testHistory

%.o : %.c
	gcc -Wall -c $<

testVersion : testVersion.c version.o
	gcc -g -Wall $^ -o $@

testOffset : testOffset.c commit.o version.o
	$(CC) $^ -o $@
	
testHistory : testHistory.c version.o commit.o history.o
	$(CC) $^ -o $@
	
check : 
	for f in *.c *.h ; do
		$(CHECK_PATCH) -f $$f
	done

clean :
	rm -rf *.o testVersion testOffset testHistory