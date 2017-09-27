.PHONY: test build examples

build:
	cmake . -Bbuild

test:
	make build && cd build && make && ./test_output

examples:
	make build && cd build && make && ./ex_output

clean/force:
	rm -r build; rm src/lexical/lex.yy.cc; rm src/ast/y.tab.*; rm src/ast/y.output
