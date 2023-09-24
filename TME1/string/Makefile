all:head main
	g++ -o tme1 *.o
	gdb tme1
head:
	g++ --std=c++11 -o0 -g3 -Wall -c -o strutil.o strutil.cpp

main:
	g++ --std=c++11 -o0 -g3 -Wall -c -o main.o main.cpp

clean:
	rm *.o
	rm tme1