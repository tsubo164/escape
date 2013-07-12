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

  print("------------------------\n");

  i = 0;
  for (;;) {
    if (i%2 == 1) {
      i++;
      continue;
    }

    vardump i;

    if (i == 10)
      break;

    i++;
  }

  return 0;
}
