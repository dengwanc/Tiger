# A.1 Lexical Noun

* __identifier__ with `_` or `$` or `?` or alphabet word 
* __block comment__ start with `/*` end with `*/`
* __line__ comment with `//`

# A.2 Data Type

* Bulit-in
```
string t;
int a;
float precent;
```

* Record(actual is a pointer)
```
Mixin {
    int head,
    Mixin tail
}
```

* Array
```
string[] cons;
```

* Function
```
// with return type
int getId(float c) {
    c! c * 10;
    c! c > 533 ? 1 : 0;

    return c + 45
}
 
// shorthand without return
int getId(float c) c + 45
 
// without return type
printint(int c) print(c)

```

# A.3 Scope Rules(basic block)

* Function Body
* Function Arguments
* While Body
* Closure Scope
```
int f(int a) {
    int d! 45;
    ()print(d)() // 45
}
```

# A.4 Expressions

* `left value` can be assign to new value(basic a memory place)
```
a! 4 // normal var
b.c! 4 // record field
s[4]! 45 // array element
```

* `left value` expression return itself
* void expressions
```
while 1 {
    a++
} // value is nothing
```
* `null` belongs all pointer type
* const number `45`, `43.34`
* const string `'hello world'`
* function call `f(1, 3)`
* negative number `-45`
* arithmetic calculation `*`, `/`， `a + b`, `-` 
* compare `a > p`, `=`, `<`, `>=`
* bool `a | b`, `&`
* record expression
```
// shorthand
List {
    int a,
    string b
} c! { 45, "ssss"} // value is c
 
List d // value is d
d.a! 45
d.b! "ssss"
```

* array expression
```
int[] a! 8 of 2 // length of a is 8 and all a[i] inited with 2
List[] b // init daynamic array
```

* pointer value assign expression
```
List {
    int a,
    float c
} l1, l2
 
l1! { 1, 2.4 }
 
l2! { 3, 2.43 }
 
l1! l2 // value copy
l1.a! 33
l2.a = 33 // true
 
l1!! l2 // deep copy
l1.a! 33
l2.a = 3 // true
 
int[] n! 8 of 2
int[] cn! n // value copy
int[] dn!! n // deep cppy
```

* assign expression
```
auto v! 5 // with declare, value is 5
 
v! 434 // normal assign, value is 434
```

* `? : ` and `?` expression
```
// simple
a = 4 ? a! 5 : a! 6;
// or 
a! a = 4 ? 5 : 6;
 
// complex
a = 4 ? {
    a+1;
    a-1;
    print(1)
} : {
    a-1;
    a+1
}
 
// only ?
a = 4 ? a! 45;
a = 4 ? {
	a! a + 1;
	a! a + a
}
```

* block expression
```
a = {
	auto b = 43;
	print('a');
	1 + b
} // value is 44
 
print(b) // Error cant find id `b`
```

* `break`
```
while 1 {
    while 1 {
       break
   	}
    print(1)
}
```

* parentheses
```
auto a! 4;
auto b! 5;
a! a + b * 4; // 24 
a! (a + b) * 4; // 40
```

* pattern match
```
x match {
    5555 => x! x+1
    6666 => printf()
    7777 => x! y + d
    else => print('done')
}
```

