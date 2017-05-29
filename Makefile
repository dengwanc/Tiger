.PHONY: test build

build:
	cmake . -Bbuild

test:
	make build; cd build; make; ./test_output;

clean/force:
	rm -r build;