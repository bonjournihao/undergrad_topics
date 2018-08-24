simulator.out : city.o vector.o main.o
	g++ -ansi -Wall -g city.o vector.o main.o -o simulator.out
main.o : main.cpp vector.h
	g++ -ansi -Wall -g -c main.cpp
vector.o : vector.cpp vector.h city.h
	g++ -ansi -Wall -g -c vector.cpp
city.o : city.cpp city.h
	g++ -ansi -Wall -g -c city.cpp 