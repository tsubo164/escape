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
    break;
  case 1:
    print("one\n");
    foo--;
    print("oneone\n");
    break;
  default:
    print("default\n");
    print("defaultdefault\n");
    break;
  }

  return 0;
}
