all: build build/bf

build: Makefile
	mkdir -p build

build/bf: build main.cpp Makefile
	g++ main.cpp -o build/bf

clean:
	rm -rf build

.PHONY: all clean
