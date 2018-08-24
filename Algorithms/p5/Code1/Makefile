simulator.out: city.o main.o citynode.o citylist.o plane.o
	g++ -Wall -ansi -g -lm -o simulator.out city.o main.o citynode.o citylist.o plane.o 

city.o: city.cpp city.h 
	g++ -Wall -ansi -g -c city.cpp

main.o: main.cpp citylist.h 
	g++ -Wall -ansi -g -c main.cpp

citylist.o: citylist.cpp citylist.h citynode.h
	g++ -Wall -ansi -g -c citylist.cpp

citynode.o: citynode.cpp citynode.h 
	g++ -Wall -ansi -g -c citynode.cpp

plane.o: plane.cpp plane.h
	g++ -Wall -ansi -g -c plane.cpp

clean:
	rm -f simulator.out city.o main.o vector.o plane.o 

