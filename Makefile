objects = BPNetwork.o main.o
CPP = g++
BPNetwork.exe:	$(objects)
				$(CPP) -g -o BPNetwork.exe $(objects)
BPNetwork.o:	BPNetwork.h BPNetwork.cpp
				$(CPP) -g -c BPNetwork.cpp
main.o:			BPNetwork.h BPNetwork.cpp main.cpp
				$(CPP) -g -c main.cpp
clean:			
				del BPNetwork.exe $(objects)
				