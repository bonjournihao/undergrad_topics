checker.out : checker.o CheckerRunner.o  
	g++ -ansi -Wall -g -o checker.out checker.o CheckerRunner.o  

checker.o : checker.cpp CheckerRunner.h checker.h 
	g++ -ansi -Wall -g -c checker.cpp

CheckerRunner.o : CheckerRunner.cpp CheckerRunner.h checker.h CPUTimer.h 
	g++ -ansi -Wall -g -c CheckerRunner.cpp

clean : 
	rm -f checker.out checker.o  CheckerRunner.o   
