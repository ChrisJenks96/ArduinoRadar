all:
	g++ -g -c primitives.cpp
	g++ -g -c core.cpp
	g++ -g -c scanner.cpp
	g++ -g -c main.cpp
	g++ -o main -w primitives.o core.o scanner.o main.o -lX11 -lGL -lGLU
clean:
	rm primitives.o core.o scanner.o main.o
