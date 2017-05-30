# Code Conventions

### named
* `#define` MUST upper join by '_'
* method must Beginning of verb & CamelCase
* Type must Capital letter begins

# Contribution Guide

> The behind list is ordered as importance.
Before coding, __discuss__ is better.
If feasible by discuss, __design__ is better.
Then the coding work.

- issue template
- pull request template
- a function at least own one unit test
- all *.tig files must passed in every module

# Develop Guide

- commit prefix usage
```
* ${description} // means modified/fixed/refactor
+ ${description} // means new feature/function
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

- run unit test
```
make test
```

- run examples
```
make ex
```

# Refrence

- [Modern Compiler Implementation in C](https://www.cs.princeton.edu/~appel/modern/c/)
- [old C version source code on branch c-version](https://github.com/dengwanc/Tiger/tree/c-version)
