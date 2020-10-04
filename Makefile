CXX=gcc
CXX_FLAGS=-g -std=c17
VALGRIND=valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes
.PHONY: all clean
TARGETS=pzip

all: $(TARGETS)

clean:
	rm -rf *~ *.o *.dSYM
