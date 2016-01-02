/* escape data types program */
/*
   $ src/ecc samples/ex008.enum.es
   $ ./a.out
*/

enum token_kind {
  number = 256;
  identifier;
  keyword;
  unknown;
};

fn main() int
{
  var a int = number;
  var b int = identifier;
  var c int = keyword;
  var d int = unknown;

  vardump a;
  vardump b;
  vardump c;
  vardump d;

  return 0;
}
