# Makefile

all: encode decode clean

encode: encode.o UniversalCoder/UniversalCoderOmega.o UniversalCoder/UniversalCoderFib.o UniversalCoder/UniversalCoderGamma.o UniversalCoder/UniversalCoderDelta.o UniversalCoder/UniversalCoder.o
	g++ -std=c++17 -o encode encode.o UniversalCoder/UniversalCoderOmega.o UniversalCoder/UniversalCoderFib.o UniversalCoder/UniversalCoderGamma.o UniversalCoder/UniversalCoderDelta.o UniversalCoder/UniversalCoder.o

decode: decode.o UniversalCoder/UniversalCoderOmega.o UniversalCoder/UniversalCoderFib.o UniversalCoder/UniversalCoderGamma.o UniversalCoder/UniversalCoderDelta.o UniversalCoder/UniversalCoder.o
	g++ -std=c++17 -o decode decode.o UniversalCoder/UniversalCoderOmega.o UniversalCoder/UniversalCoderFib.o UniversalCoder/UniversalCoderGamma.o UniversalCoder/UniversalCoderDelta.o UniversalCoder/UniversalCoder.o

encode.o: encode.cpp
	g++ -std=c++17 -c encode.cpp 

decode.o: decode.cpp
	g++ -std=c++17 -c decode.cpp

UniversalCoder/UniversalCoderGamma.cpp: UniversalCoder/UniversalCoderGamma.cpp UniversalCoder/UniversalCoder.h
	g++ -std=c++17 -c -o UniversalCoderGamma.o UniversalCoder/UniversalCoderGamma.cpp UniversalCoder/UniversalCoder.h

UniversalCoder/UniversalCoderOmega.cpp: UniversalCoder/UniversalCoderOmega.cpp UniversalCoder/UniversalCoder.h
	g++ -std=c++17 -c -o UniversalCoderOmega.o UniversalCoder/UniversalCoderOmega.cpp UniversalCoder/UniversalCoder.h

UniversalCoder/UniversalCoderFib.cpp: UniversalCoder/UniversalCoderFib.cpp UniversalCoder/UniversalCoder.h
	g++ -std=c++17 -c -o UniversalCoderFib.o UniversalCoder/UniversalCoderFib.cpp UniversalCoder/UniversalCoder.h

UniversalCoder/UniversalCoderDelta.cpp: UniversalCoder/UniversalCoderDelta.cpp UniversalCoder/UniversalCoder.h
	g++ -std=c++17 -c -o UniversalCoderDelta.o UniversalCoder/UniversalCoderDelta.cpp UniversalCoder/UniversalCoder.h

UniversalCoder/UniversalCoder.o: UniversalCoder/UniversalCoder.cpp UniversalCoder/UniversalCoder.h
	g++ -std=c++17 -c UniversalCoder/UniversalCoder.cpp -o UniversalCoder/UniversalCoder.o

clean:
	rm -f *.o
	rm -f UniversalCoder/*.o



