/* escape var int program */
/*
   $ src/ecc samples/ex002.var_int.es
   $ ./a.out
*/

function main: int ()
{
	var foo: int = 654 - 27 * 2;
	var bar: int; // = 0

	vardump foo;

	foo = 123 + 23 * 2;
	bar = 2 + 999 * 2;
	bar = (7 - 5) * 2;

	vardump foo;
	vardump bar;

	bar = bar + foo + 1;
	vardump bar;

	return 0;
}
