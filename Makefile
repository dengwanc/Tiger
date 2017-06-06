.PHONY: test build

build:
	cmake . -Bbuild

test:
	make build; cd build; make; ./test_output;

ex:
	make build; cd build; make; ./ex_output;

compiler:
	make build; cd build; make; ./compiler_output;

clean/force:
	rm -r build;