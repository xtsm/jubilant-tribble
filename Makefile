SRCS=main.cpp curses_context.cpp util.cpp global.cpp camera.cpp
ASM=matmul.s
.PHONY: all build

all: build

build: $(SRCS) $(ASM)
	as $(ASM) -o asm.o
	g++ -std=gnu++17 -Wall -Wextra -Werror -Weffc++ -g $(SRCS) asm.o -lncurses -mavx

