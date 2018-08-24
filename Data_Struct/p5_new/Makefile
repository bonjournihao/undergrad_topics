RunCity.out: RunCity.o StreetSelecter.o
	g++ -Wall -ansi -g -o RunCity.out RunCity.o StreetSelecter.o 

RunCity.o: RunCity.cpp StreetSelecter.h RunCity.h CPUTimer.h 
	g++ -Wall -ansi -g -c RunCity.cpp

StreetSelecter.o: StreetSelecter.cpp StreetSelecter.h BinaryHeap.h vector.h
	g++ -Wall -ansi -g -c StreetSelecter.cpp

clean:
	rm -f RunCity.out RunCity.o StreetSelecter.o 
