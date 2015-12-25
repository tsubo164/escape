/* escape switch statement program */
/*
   $ src/ecc samples/ex006.switch_stmt.es
   $ ./a.out
*/

fn main() int
{
  var foo int = 0;

  switch (foo) {
  case 0:
    print("zero\n");
    foo++;
    print("zerozero\n");
  case 1:
    print("one\n");
    foo--;
    print("oneone\n");
  default:
    print("default\n");
    print("defaultdefault\n");
  }

  return 0;
}
