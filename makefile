all: inicio


inicio:
	g++ -std=c++11 cache.hpp main.cpp -o mesi.o
	
clean:
	rm *.o *.gch
