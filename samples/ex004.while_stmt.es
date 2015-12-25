/* escape while statements program */
/*
   $ src/ecc samples/ex004.while_stmt.es
   $ ./a.out
*/

fn main() int
{
  var foo int = 10;

  while (foo > 0) {
    foo = foo - 1;
    vardump foo;
  }

  do {
    foo = foo + 1;
    vardump foo;
  } while (foo < 10);

  return 0;
}
