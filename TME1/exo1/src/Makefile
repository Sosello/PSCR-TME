all:head main
	g++ -o tme1 *.o
	gdb tme1

clean:
	rm *.o
	rm tme1

head:
	g++ --std=c++11 -o0 -g3 -Wall -c -o List.o List.cpp

main:
	g++ --std=c++11 -o0 -g3 -Wall -c -o main.o main.cpp