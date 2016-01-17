/* escape data types program */
/*
   $ src/ecc samples/ex009.array.es
   $ ./a.out
*/

fn main() int
{
  var a int[3] = {111, 222, 333};
  var P double[3] = {12.3, 3.14, 111.122};

  var b int = a[2];
  var Q double = P[2];

  vardump b;
  vardump Q;

  return 0;
}
