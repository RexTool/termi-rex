.PHONY: all configure build run clean

ifeq ($(OS),Windows_NT)
ARGS = -G Ninja -DCMAKE_CXX_COMPILER=g++
APP = build/bin/app.exe
else
ARGS =
APP = ./build/bin/app
endif

all:
	$(MAKE) configure
	$(MAKE) build
	$(MAKE) run

create:
	$(MAKE) build
	$(MAKE) run
	
configure:
	cmake -S . -B build $(ARGS)

build:
	cmake --build build --parallel

run:
	"$(APP)"

clean:
	cmake -E rm -rf build
	cmake -E make_directory build
	cmake -E touch build/.gitkeep