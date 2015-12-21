/*
 * escape hello world program
 *   $ src/ec samples/ex001.hello.es
 *   $ ./a.out
 */

fn main() int
{
	//print("Hello, world!\n");
	return 0;
}

var i = 0;
var n int; // = 0
var hoge int = 123;
var foo int = (999111 + n) * hoge;

break;
continue;
return;
continue;
break;
return 123;

return 1++;
return 1--;
return ++93;
return --88;
return a++;
return a * 23.3 % b / 3;
return i * 3.14 - j / 120.0f;
return j << 8;
return k >> 8;
return j < k;
return j > k * 8;
return j == k * s;
return x & y - z;
return x & y ^ z;
return x | y ^ z;
return x && y;
return x || y;
return a = b = c;

var i = 0;
var n int; // = 0
var hoge int = 123;
var foo int = (999111 + n) * hoge;

goto error;
label hoge:
continue;

{
  var n int = 333;
  label a:
  return;
  /*
  n = 23.f * (2 + foo);
  n = 23;
  */
}

while (n < 10) {
  var n int = 333;
  continue;
}

do return; while (i < 23);

for (var i = 0; i < N; i++) {
  var n = 23.f * (2 + foo);
  var n = 23;
}

if (a == 2) {
  var b = 32.1;
} else {
  var c = 92.1;
  b = c;
}

switch (a) {
case 123: case 999:
case 213: case 888:
case 132: case 666:
  break;
}

/*

hoge;
(hoge;)
*/

fn main() int
{
	//print("Hello, world!\n");
	return 0;
}
