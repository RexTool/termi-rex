.PHONY: all configure build run clean

ifeq ($(OS),Windows_NT)
GENERATOR = -G "MinGW Makefiles"
APP = build/bin/app.exe
else
GENERATOR =
APP = ./build/bin/app
endif

all:
	$(MAKE) configure
	$(MAKE) build
	$(MAKE) run

configure:
	cmake -S . -B build $(GENERATOR)

build:
	cmake --build build --parallel

run:
	"$(APP)"

clean:
	cmake -E rm -rf build
	cmake -E make_directory build
	cmake -E touch build/.gitkeep