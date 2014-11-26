### Tiger is a toy language. 
### Include:
    * nesting function
    * hidden pointer as record
    * array
    * int, string
    * kinds of control struct

### Lexical:
    * include: [Identifiers, Comments, Integer literal, String literal]
    * reserved words: [while, break, let, in, end, function, var, type, array, if, then, else, do, of, nil]
    * interpunction: , : ; ( ) { } [ ] . + - * / = <> >= <= < > & | :=
    * comment(can be nested): /* here  /* is */ content */

### Provision:
    * `xxx` means strong reserved words xxx
    *  {x}  means probility empty
    *   E   means empty string
    *  //   means about note
    
### Declare:
#### declare sequence:

    decs -> {dec}
	dec  -> tydec
	     -> vardec
	     -> fundec
	     
#### type declare:
    
       typedec -> `type` type_id = ty
            ty -> type_id
               -> {typefields}
               -> array of type_id
    
    typefields -> E
               -> id: type_id{, id: type_id}
    
        //bulit-in type: `int`, `string`
        //record type: type Node = {key: int, next: Node}
        //array type: type intArray = array of int

#### variable:
    
    vardec -> `var` id := expr
           -> `var` id: type_id := expr
           
       //if short type_id then id's type decided by invalid expr's type
       
#### function:
    
    funcdec -> function id(params) = expr
            -> function id(params): type_id = expr
    
    //if short type_id then return void

### Scope Rules:
    * `let ...vardec | typedec | funcdec... in expr end` begin vardec end in `end`
    * function params only live in function-body
    * nest scope: Tiger allow visit outer scope var if def
    * namespace: 1. Type's($ Node.int) 2. Var | Function ($ Node(), Node+1)
    * rename: the lastest def always worked
    
### Var & Left Value:

    lvalue -> id
           -> lvalue.id
           -> lvalue[int-expr]
    // lvalue is a memory addr
    // var, params, record-filed, array-index can be lvalue

### Exression:
    * lvalue:
      when a lvalue in a expr, eval is the value in the mem-addr
    


	
	
	
