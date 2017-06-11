# Code Conventions

* `#define` must upper & join by `_`
* Method name must beginning of verb & CamelCase
* Type define must capital letter begins

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

- Run unit test
```
make test
```

- Run examples
```
make ex
```

# Refrence

- [Modern Compiler Implementation in C](https://www.cs.princeton.edu/~appel/modern/c/)
- [Old C Version Source Code](https://github.com/dengwanc/Tiger/tree/c-version)
