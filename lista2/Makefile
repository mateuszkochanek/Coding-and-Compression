# Makefile

all: encode decode clean

encode: encode.o BitArr.o
	g++ -std=c++17 -o encode encode.o BitArr.o

decode: decode.o BitArr.o
	g++ -std=c++17 -o decode decode.o BitArr.o

encode.o: encode.cpp
	g++ -std=c++17 -c encode.cpp 

decode.o: decode.cpp
	g++ -std=c++17 -c decode.cpp

BitsTable.o: BitArr.cpp BitArr.h
	g++ -std=c++17 -c BitArr.cpp -o BitArr.o

clean:
	rm -f *.o
