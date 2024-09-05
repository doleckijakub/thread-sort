CC := cc

thread-sort.o: thread-sort.c | thread-sort.h
	$(CC) -o $@ -c $<

thread-sort-tester: thread-sort-tester.c thread-sort.o
	$(CC) -o $@ $^

test: thread-sort-tester
	./thread-sort-tester
