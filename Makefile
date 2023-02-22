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
	${CC}  *.cpp  -o main.o ${STANDARD} ${INCLUDE}


log:
	@echo "Compile..."
	${CC}  *.cpp  -o main.o ${STANDARD} ${INCLUDE} -D logYard

draw:
	./main.o > "temp.gv.txt"
	dot "temp.gv.txt" -Tpng:cairo:gd -Gcharset=utf8 > "graph.png"
	rm "temp.gv.txt"

clean:
	@echo "Cleaning up..."
	rm -rvf *.o ${BINS}