# Code Conventions

* `#define` must upper & join by `_`
* Method name must beginning of verb & CamelCase
* Type define must capital letter begins
* Before you actually figure out what happended DO NOT USE those below
	* disable STL
	* disable std::string

* Recommendatory Features
	* Lambda
	* `auto` keyword
	* `decltype`
	* `nullptr`
	* `virtual` as interface
	* Function Overloading

# Contribution Guide

> The behind list is ordered as importance.
Before coding, __discuss__ is better.
If feasible by discuss, __design__ is better.
Then the coding work.

- Issue template
- Pull request template
- A function at least own one unit test
- All *.tig files must passed in every module



# Develop Guide

- Commit prefix usage
```
* ${description} // means modified/fixed
+ ${description} // means new feature/refactor/optimization
- ${description} // means remove feature/unsupport sth
# ${description} // means polish meta info(ReadME/History)
/* 
 * ----------------
 * --- examples ---
 * ----------------
 */
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

# Refrence

- [Modern Compiler Implementation in C](https://www.cs.princeton.edu/~appel/modern/c/)
- [Old C Version Source Code](https://github.com/dengwanc/Tiger/tree/c-version)
- [LLVM Tutorial: Table of Contents](http://llvm.org/docs/tutorial/index.html)
