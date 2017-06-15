.PHONY: test build

build:
	cmake . -Bbuild

test:
	make build && cd build && make && ./test_output

ex:
	make build && cd build && make && ./ex_output

clean/force:
	rm -r build; rm src/lexical/lex.yy.cc; rm src/syntax/y.tab.*
