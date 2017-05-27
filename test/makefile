# holy fucking shit  linker on sucking Mac
# https://stackoverflow.com/questions/16352833/linking-with-clang-on-os-x-generates-lots-of-symbol-not-found-errors
CC = clang++ -std=c++11 -stdlib=libc++

output: main.o framework.spec.o
	$(CC) -o output main.o framework.spec.o

framework.spec.o:
	$(CC) -c framework.spec.cc

main.o:
	$(CC) -c main.cc

clean:
	rm *.o output

test:
	make clean; make; ./output; make clean;


