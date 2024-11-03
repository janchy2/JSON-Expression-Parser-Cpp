A **C++** console application that allows evaluating expressions on a **JSON** file.

Example of usage:  

 - test.json is a JSON file with the content:
{"a": { "b": [ 1, 2, { "c": "test" }, [11, 12] ]}}
- ./json_eval is the console application

The application supports:

- Trivial expressions with JSON paths:

  $ ./json_eval test.json "a.b[1]"
  
  2

  $ ./json_eval test.json "a.b[2].c"
  
  test
  
  $ ./json_eval test.json "a.b"
  
  [ 1, 2, { "c": "test" } ]
  
- Expressions in the subscript operator []:
  
  $ ./json_eval test.json "a.b[a.b[1]].c"
  
  test

- Intrinsic functions: min, max, avg, sum, size:

  min, max, avg and sum return the min, max element, avg of or sum of elements from the passed array or arguments.
  
  $ ./json_eval test.json "max(a.b[0], a.b[1])"
  
  2
  
  $ ./json_eval test.json "min(a.b[3])"
  
  11
  
  size - returns size of the passed object, array or string
  
  $ ./json_eval test.json "size(a)"
  
  1
  
  $ ./json_eval test.json "size(a.b)"
  
  4
  
  $ ./json_eval test.json "size(a.b[a.b[1]].c)"
  
  4
  

- Number literals:

  $ ./json_eval test.json "max(a.b[0], 10, a.b[1], 15)"
  
  15

- Arithmetic binary operators: +, -, *, /:

  $ ./json_eval test.json "a.b[0] + a.b[1]"
  
  3
  
The application uses async tasks to speed up the evaluation of operator expressions.
