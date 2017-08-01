## Code Conventions

* `#define` must upper & join by `_`
* Method name must beginning of verb & CamelCase
* Type define must capital letter begins
* Before you actually figure out what happened DO NOT USE:
	* disable STL
	* disable std::string

* Recommendatory Features
	* Lambda
	* `auto`, `decltype`
	* `nullptr`
	* `virtual` as interface
	* Function Overloading

## Contribution Guide

> This list is ordered by importance

- Issue template
- Pull request template
- Assume architecture is basing on black-box
- A Interface at least own one unit test
- All *.tig files must test passed in every module



## Develop Guide

- Commit prefix usage
```
* ${description} // means modified/fixed
+ ${description} // means new feature/refactor/optimization
- ${description} // means remove feature/unsupport sth
# ${description} // means polish meta info(ReadME/History)
 
// Examples
 
$ git co '+ support template String complie to AST'
$ git co '# add dev scripts to readMe'
```

- Run unit test
```
make test
```

- Run examples
```
make examples
```

## Refrence

- [Modern Compiler Implementation in C](https://www.cs.princeton.edu/~appel/modern/c/)
- [Old C Version Source Code](https://github.com/dengwanc/Tiger/tree/c-version)
- [LLVM Tutorial: Table of Contents](http://llvm.org/docs/tutorial/index.html)
