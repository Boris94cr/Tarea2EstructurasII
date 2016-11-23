all: inicio


inicio:
	g++ -std=c++11 cache.hpp main.cpp -o mi_cache.o
	
clean:
	rm *.o *.gch
