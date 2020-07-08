select1:select1.c
	gcc $^ -o $@

all:tcp_cli http_srv
http_srv:http_srv.cpp
	g++ $^ -o $@ -lpthread
thread_srv:thread_srv.cpp
	g++ $^ -o $@ -lpthread
process_srv:process_srv.cpp
	g++ $^ -o $@ 
tcp_cli:tcp_cli.cpp
	g++ $^ -o $@ 
tcp_srv:tcp_srv.cpp
	g++ $^ -o $@

all:udp_cli udp_srv
udp_cli:udp_cli.cpp
	g++ $^ -o $@ 
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
