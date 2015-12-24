/* escape data types program */
/*
   $ src/ecc samples/ex007.data_types.es
   $ ./a.out
*/

fn main() int
{
  /*
  var b: bool   = 0;
  var c: char   = 't';
  var h: short  = 0x12;
  */
  var i int = 12;
  var l long   = 1343241324L;
  var f float  = 2.1828f;
  var d double = 3.14159265352384626433;

  /*
  vardump b;
  vardump c;
  vardump h;
  */
  vardump i;
  vardump l;
  vardump f;
  vardump d;

  return 0;
}
