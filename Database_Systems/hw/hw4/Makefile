all: load.out query.out

load.out: test.cpp
	g++ -Wall -ansi -g -o load.out test.cpp -lpqxx -lpq

query.out: query.cpp
	g++ -Wall -ansi -g -o query.out query.cpp -lpqxx -lpq

clean: 
	rm -f load.out query.out