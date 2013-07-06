/* escape var if statements program */
/*
   $ src/ecc samples/ex003.if_stmt.es
   $ ./a.out
*/

function main: int ()
{
  var foo: int = 2;
  var bar: int = 1;

  foo = 8 - 4 * 3 - 1;

  bar = 4 + foo^23 <= bar * 2 || 4 - foo && 3 | 23 << 3 & bar > 2;

  vardump foo;
  vardump bar;

  if ((foo | bar) == 3) {
    print("true\n");
    vardump foo;
  }
  else if (foo)
    vardump bar;

  return 0;
}
