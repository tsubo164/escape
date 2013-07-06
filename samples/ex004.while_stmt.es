/* escape if statements program */
/*
   $ src/ecc samples/ex003.if_stmt.es
   $ ./a.out
*/

function main: int ()
{
  var foo: int = 10;

  while (foo > 0) {
    foo = foo - 1;
    vardump foo;
  }

  return 0;
}
