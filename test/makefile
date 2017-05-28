# holy fucking shit  linker on sucking Mac
# https://stackoverflow.com/questions/16352833/linking-with-clang-on-os-x-generates-lots-of-symbol-not-found-errors
CC = clang++ -std=c++11

output: main.o utils.o utils.spec.o
	$(CC) main.o  utils.o utils.spec.o -o output

utils.o:
	$(CC) -c ../src/utils/index.cc -o utils.o

utils.spec.o:
	$(CC) -c utils.spec.cc

main.o:
	$(CC) -c main.cc

clean:
	rm *.o output

test:
	make clean; make; ./output; make clean;


