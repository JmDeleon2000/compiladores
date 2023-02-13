# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects

.PHONY = all clean

CC = g++			# compiler to use

LINKERFLAG = -lm

STANDARD = -std=c++17
INCLUDE = -I./headers
PERMISSIVE = -Wwrite-strings

compile:
	@echo "Compile..."
	${CC}  *.cpp  -o main.o ${STANDARD} ${INCLUDE} ${PERMISSIVE}

clean:
	@echo "Cleaning up..."
	rm -rvf *.o ${BINS}