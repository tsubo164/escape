#include "stream.h"
#include "unit_test.h"
#include <stdio.h>

int main()
{
  const char filename[] = "stream_test.es";
  {
    FILE *file = fopen(filename, "w");
    const char src[] = "this\nis\ntest\n";
    fprintf(file, "%s", src);
    fclose(file);
  }

  {
    const char src[] = " abcd 12.3 /* ... */  \n this is stream?";
    struct stream strm = STREAM_INIT;
    char c;

    open_string_stream(&strm, src);

    c = stream_getc(&strm);
    TEST_INT(c, ' ');

    c = stream_getc(&strm);
    TEST_INT(c, 'a');

    c = stream_ungetc(&strm);
    TEST_INT(c, ' ');

    c = stream_getc(&strm);
    TEST_INT(c, 'a');

    c = stream_getc(&strm);
    TEST_INT(c, 'b');

    c = stream_getc(&strm);
    TEST_INT(c, 'c');

    c = stream_getc(&strm);
    TEST_INT(c, 'd');

    c = stream_ungetc(&strm);
    TEST_INT(c, 'c');

    c = stream_ungetc(&strm);
    TEST_INT(c, 'b');

    c = stream_ungetc(&strm);
    TEST_INT(c, 'a');

    c = stream_getc(&strm);
    TEST_INT(c, 'b');

    c = stream_getc(&strm);
    TEST_INT(c, 'c');

    c = stream_getc(&strm);
    TEST_INT(c, 'd');

    c = stream_getc(&strm);
    TEST_INT(c, ' ');

    close_stream(&strm);
  }
  {
    struct stream strm;
    char c;

    open_file_stream(&strm, filename);

    c = stream_getc(&strm);
    TEST_INT(c, 't');

    c = stream_getc(&strm);
    TEST_INT(c, 'h');

    c = stream_getc(&strm);
    TEST_INT(c, 'i');

    c = stream_getc(&strm);
    TEST_INT(c, 's');

    c = stream_getc(&strm);
    TEST_INT(c, '\n');

    c = stream_ungetc(&strm);
    TEST_INT(c, 's');

    c = stream_ungetc(&strm);
    TEST_INT(c, 'i');

    c = stream_ungetc(&strm);
    TEST_INT(c, 'h');

    c = stream_getc(&strm);
    TEST_INT(c, 'i');

    c = stream_getc(&strm);
    TEST_INT(c, 's');

    c = stream_getc(&strm);
    TEST_INT(c, '\n');

    c = stream_getc(&strm);
    TEST_INT(c, 'i');

    c = stream_getc(&strm);
    TEST_INT(c, 's');

    c = stream_getc(&strm);
    TEST_INT(c, '\n');

    c = stream_getc(&strm);
    TEST_INT(c, 't');

    c = stream_getc(&strm);
    TEST_INT(c, 'e');

    c = stream_getc(&strm);
    TEST_INT(c, 's');

    c = stream_getc(&strm);
    TEST_INT(c, 't');

    c = stream_getc(&strm);
    TEST_INT(c, '\n');

    c = stream_getc(&strm);
    TEST_INT(c, '\0');

    c = stream_getc(&strm);
    TEST_INT(c, '\0');

    close_stream(&strm);
  }
#if 0
  {
    const char src[] = " abcd 12.3 /* ... */  \n this is stream?";
    struct stream *strm = string_stream(src);
    char c;

    c = stream_getc(strm);
    TEST_INT(c, ' ');

    c = stream_getc(strm);
    TEST_INT(c, 'a');

    c = stream_ungetc(strm);
    TEST_INT(c, ' ');

    c = stream_getc(strm);
    TEST_INT(c, 'a');

    c = stream_getc(strm);
    TEST_INT(c, 'b');

    c = stream_getc(strm);
    TEST_INT(c, 'c');

    c = stream_getc(strm);
    TEST_INT(c, 'd');

    c = stream_ungetc(strm);
    TEST_INT(c, 'c');

    c = stream_ungetc(strm);
    TEST_INT(c, 'b');

    c = stream_ungetc(strm);
    TEST_INT(c, 'a');

    c = stream_getc(strm);
    TEST_INT(c, 'b');

    c = stream_getc(strm);
    TEST_INT(c, 'c');

    c = stream_getc(strm);
    TEST_INT(c, 'd');

    c = stream_getc(strm);
    TEST_INT(c, ' ');

    free_stream(strm);
  }
  {
    struct stream *strm = file_stream(filename);
    char c;

    c = stream_getc(strm);
    TEST_INT(c, 't');

    c = stream_getc(strm);
    TEST_INT(c, 'h');

    c = stream_getc(strm);
    TEST_INT(c, 'i');

    c = stream_getc(strm);
    TEST_INT(c, 's');

    c = stream_getc(strm);
    TEST_INT(c, '\n');

    c = stream_ungetc(strm);
    TEST_INT(c, 's');

    c = stream_ungetc(strm);
    TEST_INT(c, 'i');

    c = stream_ungetc(strm);
    TEST_INT(c, 'h');

    c = stream_getc(strm);
    TEST_INT(c, 'i');

    c = stream_getc(strm);
    TEST_INT(c, 's');

    c = stream_getc(strm);
    TEST_INT(c, '\n');

    c = stream_getc(strm);
    TEST_INT(c, 'i');

    c = stream_getc(strm);
    TEST_INT(c, 's');

    c = stream_getc(strm);
    TEST_INT(c, '\n');

    c = stream_getc(strm);
    TEST_INT(c, 't');

    c = stream_getc(strm);
    TEST_INT(c, 'e');

    c = stream_getc(strm);
    TEST_INT(c, 's');

    c = stream_getc(strm);
    TEST_INT(c, 't');

    c = stream_getc(strm);
    TEST_INT(c, '\n');

    c = stream_getc(strm);
    TEST_INT(c, '\0');

    c = stream_getc(strm);
    TEST_INT(c, '\0');

    free_stream(strm);
  }
#endif

	printf("%s: %d/%d/%d: (FAIL/PASS/TOTAL)\n", __FILE__,
		TestGetFailCount(), TestGetPassCount(), TestGetTotalCount());

	return 0;
}
