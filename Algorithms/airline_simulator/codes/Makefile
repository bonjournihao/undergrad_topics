simulator.out: city.o main.o plane.o
	g++ -Wall -ansi -o simulator.out city.o main.o plane.o 

city.o: city.cpp city.h 
	g++ -Wall -ansi -c city.cpp

main.o: main.cpp list.h list.cpp airport.h
	g++ -Wall -ansi -c main.cpp

plane.o: plane.cpp plane.h
	g++ -Wall -ansi -c plane.cpp

clean:
	rm -f simulator.out city.o main.o plane.o 
