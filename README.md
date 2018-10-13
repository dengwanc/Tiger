> For Learning Compiler Technology

## Code Conventions

* Method name must beginning of verb & CamelCase
* Type define must capital letter begins
* DO NOT USE: `void*`
* Recommendatory Features
	* Lambda Expressions
	* `auto`, `decltype`
	* `nullptr`
	* `virtual` as interface
	* Function Overloading
* A Interface at least own one unit test
* All *.tig files must test passed in every module

## Develop Guide

- Required
	* flex 2.5.35 Apple(flex-31)
	* bison bison (GNU Bison) 3.0.4
	* clang Apple LLVM version 8.1.0 (clang-802.0.42)
	* cmake 3.8.1

- Commit prefix usage
```
* ${description} # means modified/fixed
+ ${description} # means new feature/refactor/optimization
- ${description} # means remove feature/unsupport sth
# ${description} # means polish meta info(ReadME/History)
 
$ git co '+ support template string complie to AST'
$ git co '# add dev scripts to ReadMe'
```

- Running unit test
```
make test
```

- Running examples
```
make examples
```

## Reference

- [Modern Compiler Implementation in C](https://www.cs.princeton.edu/~appel/modern/c/)
- [Old C Version Source Code](https://github.com/dengwanc/Tiger/tree/c-version)
- [LLVM Tutorial: Table of Contents](http://llvm.org/docs/tutorial/index.html)
