.PHONY: configure build run clean

all:
	$(MAKE) configure
	$(MAKE) build
	$(MAKE) run
configure:
	cmake -S . -B build

build:
	cmake --build build --parallel

run: build
	./build/bin/app

clean:
	rm -rf build
	mkdir -p build