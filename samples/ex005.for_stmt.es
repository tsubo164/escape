/* escape for statement program */
/*
   $ src/ecc samples/ex005.for_stmt.es
   $ ./a.out
*/

function main: int ()
{
  var foo: int = 10;
  var bar: int;
  var i: int;

  foo = 210 % 100;
  vardump foo;

  bar = ++foo;
  vardump bar;

  bar = foo--;
  vardump bar;

  bar++;
  vardump bar;

  for (i = 0; i < 5; i++) {
    vardump i;
  }

  return 0;
}
