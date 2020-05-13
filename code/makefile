testBuild: mechanisms.o geometry.o region.o bitmap.o claw.o revolver.o fairways.o arena.o arenabject.o constants.o
	g++ -o mechanisms.o geometry.o region.o bitmap.o claw.o revolver.o fairways.o arena.o arenaobject.o constants.o
bitmap.o: bitmap.cpp bitmap.h geometry.h region.h
	g++ -std=c++17 -c bitmap.cpp

