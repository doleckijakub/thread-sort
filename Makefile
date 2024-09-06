CC := cc

thread-sort.o: thread-sort.c | thread-sort.h
	$(CC) -o $@ -c $<

thread-sort-tester: thread-sort-tester.c thread-sort.o
	$(CC) -o $@ $^

test: thread-sort-tester
	./thread-sort-tester 2 1
	./thread-sort-tester 2 3 1
	./thread-sort-tester 9 1 2 4 3 5 8 6 0 7
