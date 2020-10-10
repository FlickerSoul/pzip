CXX=gcc
CXX_FLAGS=-g -std=c17
VALGRIND=valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes
.PHONY: all clean
TARGETS=pzip

all: $(TARGETS)

pzip_utils.o: pzip_utils.c pzip_utils.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

task_queue.o: task_queue.c task_queue.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

test_utils.o: test_utils.c test_utils.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

test_task_queue.o: test_task_queue.c task_queue.o test_utils.o
	$(CXX) $(CXX_FLAGS) -o $@ $^

test_task_queue: test_task_queue.o
	./test_task_queue.o

pzip: pzip.c pzip_utils.o
	$(CXX) $(CXX_FLAGS) -o $@ $^

run: pzip
	./pzip

clean:
	rm -rf *~ *.o *.dSYM $(TARGETS)
