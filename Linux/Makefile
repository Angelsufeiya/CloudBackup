udp_srv:udp_srv.c
	gcc $^ -o $@ 
threadpool:threadpool.cpp
	g++ $^ -o $@ -lpthread
customer_producter:customer_productor.cpp
	g++ customer_productor.cpp -o customer_productor
productor_customer:productor_customer.cpp
	g++ productor_customer.cpp -o productor_customer
bowl:bowl.c
	gcc bowl.c -o bowl
ticket:ticket.c
	gcc ticket.c -o ticket
pthread:pthread.c
	gcc pthread.c -o pthread
signal:signal.c
	gcc signal.c -o signal
dup2:dup2.c
	gcc dup2.c -o dup2
test:test.c
	g++ test.c -o test

.PHONY:clean
clean:
	rm -f test.c test
