main: main.cc ant.o utils.o
	g++ main.cc ant.o utils.o -o main -lsfml-graphics -lsfml-window -lsfml-system

utils.o: utils.cc utils.hh
	g++ -c utils.cc -o utils.o

ant.o: ant.cc ant.hh
	g++ -c ant.cc -o ant.o