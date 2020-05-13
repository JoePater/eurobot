mechTest: main.o revolver.o claw.o mechanisms.o
	g++ -o mechTest main.o revolver.o claw.o mechanisms.o
	
main.o: main.cpp revolver.h claw.h mechanisms.h
	g++ -c main.cpp
	
revolver.o: revolver.cpp revolver.h
	g++ -c revolver.cpp
	
claw.o: claw.cpp claw.h
	g++ -c claw.cpp
	
mechanisms.o: mechanisms.cpp mechanisms.h revolver.h claw.h
	g++ -c mechanisms.cpp
