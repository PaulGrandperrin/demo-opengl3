CC=clang++
CFLAGS= -O3 -Wall -Wextra -pedantic -Wno-local-type-template-args -Wno-unused-parameter -Wno-logical-op-parentheses -std=c++11
LDFLAGS= -lGLEW -lIL -lSDL -lGL

all: demo

demo: src/main.o src/graphicEngine.o
	$(CC) $(LDFLAGS) src/main.o src/graphicEngine.o -o demo

src/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c src/main.cpp -o src/main.o

src/graphicEngine.o: src/graphicEngine.cpp src/graphicEngine.hpp
	$(CC) $(CFLAGS) -c src/graphicEngine.cpp -o src/graphicEngine.o

clean:
	rm -f src/*.o demo

