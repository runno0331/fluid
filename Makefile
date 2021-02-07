CXX = g++
CFLAGS = -g -Wall
LDLIBS = -lglut -lGL -lGLU

ALL: main.o fluid.o field.o matrix.o
	$(CXX) $(CFLAGS) -o main main.o fluid.o field.o matrix.o $(LDLIBS)
	rm *.o

main.o: main.cpp
	$(CXX) $(CFLAGS) -o main.o -c main.cpp $(LDLIBS)

fluid.o: fluid.cpp
	$(CXX) -o fluid.o -c fluid.cpp $(LDLIBS)

field.o: field.cpp
	$(CXX) -o field.o -c field.cpp

matrix.o: matrix.cpp
	$(CXX) -o matrix.o -c matrix.cpp

clean:
	rm main