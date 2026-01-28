all: build

build:
	g++ -std=c++23 \
    -Wall -Wextra -pedantic -Werror -Wcast-qual -Wconversion-null -Wmissing-declarations -Woverlength-strings -Wpointer-arith -Wunused-local-typedefs -Wunused-result -Wvarargs -Wvla -Wwrite-strings \
	src/*.cpp
