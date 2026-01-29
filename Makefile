all: build

build:
	clang++ -std=c++23 -glldb -fno-omit-frame-pointer -Og \
    -Wall -Wextra -pedantic -Werror -Wcast-qual -Wconversion-null -Wmissing-declarations -Woverlength-strings -Wpointer-arith -Wunused-local-typedefs -Wunused-result -Wvarargs -Wvla -Wwrite-strings \
	src/*.cpp -o out
