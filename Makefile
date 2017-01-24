all: functions.o main.o
	g++ -o rapcp functions.o main.o


functions.o: functions.cpp functions.hpp
	g++ -c functions.cpp 


main.o: main.cpp
	g++ -c main.cpp

