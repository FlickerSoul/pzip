CXX=gcc
CXX_FLAGS=-g -std=c17 -pthread -O2
VALGRIND=valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes
.PHONY: all clean
TARGETS=pzip

all: $(TARGETS)

locks.o: locks.c locks.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<
	
constants.o: constants.c constants.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

pzip_utils.o: pzip_utils.c pzip_utils.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

task_queue.o: task_queue.c task_queue.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

file_worker_utils.o: file_worker_utils.c file_worker_utils.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

file_worker.o: file_worker.c file_worker.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

compression_worker.o: compression_worker.c compression_worker.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

test_utils.o: test_utils.c test_utils.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

test_task_queue.o: test_task_queue.c task_queue.o test_utils.o constants.o
	$(CXX) $(CXX_FLAGS) -o $@ $^

test_workers.o: test_workers.c compression_worker.o file_worker_utils.o constants.o locks.o task_queue.o
	$(CXX) $(CXX_FLAGS) -o $@ $^

test_task_queue: test_task_queue.o
	./test_task_queue.o

test_workers: test_workers.o
	./test_workers.o

pzip: pzip.c pzip_utils.o task_queue.o file_worker_utils.o file_worker.o compression_worker.o locks.o constants.o
	$(CXX) $(CXX_FLAGS) -o $@ $^

run: pzip
	./pzip

clean:
	rm -rf *~ *.o *.gch *.dSYM $(TARGETS)
