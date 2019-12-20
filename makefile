md5: main.o md5.o
	g++ main.o md5.o -o md5
main.o: main.cpp md5.h
	g++ -c main.cpp -o main.o -std=c++11
md5.o: md5.cpp
	g++ -c md5.cpp -o md5.o -std=c++11

clean:
	rm *.o
