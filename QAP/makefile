bin = ./bin
obj = ./obj
src = ./src
inc = ./include
lib = ./lib
doc = ./doc
flag = -O2

all: $(bin)/qap

debug: flag := -g
debug: $(bin)/qap

$(bin)/qap: $(obj)/practica1.o $(obj)/problema.o $(obj)/solucion.o
	g++ $(flag) -o $(bin)/qap $(obj)/problema.o $(obj)/solucion.o $(obj)/practica1.o -std=c++11

$(obj)/solucion.o: $(src)/solucion.cpp $(inc)/solucion.h
	g++ $(flag) -c -o $(obj)/solucion.o $(src)/solucion.cpp -I$(inc) -std=c++11

$(obj)/practica1.o: $(src)/practica1.cpp $(inc)/problema.h 
	g++ $(flag) -c -o $(obj)/practica1.o $(src)/practica1.cpp -I$(inc) -std=c++11

$(obj)/problema.o: $(src)/problema.cpp
	g++ $(flag) -c -o $(obj)/problema.o $(src)/problema.cpp -I$(inc) -std=c++11

clean:
	rm $(obj)/*.o

documentacion:
	doxygen ./doc/doxys/Doxyfile

	

