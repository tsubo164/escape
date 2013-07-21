#include "lexer.h"
#include "Test.h"
#include <stdio.h>

int main()
{
	{
		struct Lexer *lexer = NULL;
		struct Token token;

		const char input_string[] = ""
		"/* escape minimum program */\n"
		"\n"
		"function main: int ()\n"
		"{\n"
		"	return 0;\n"
		"}\n";

		lexer = Lexer_New();
		Lexer_SetInputString(lexer, input_string);

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, TK_FUNCTION);

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, TK_IDENTIFIER);
		TEST_STR(token.value.name, "main");

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, ':');

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, TK_INT);

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, '(');

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, ')');

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, '{');

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, TK_RETURN);

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, TK_NUMBER);
		TEST_DOUBLE(token.value.number, 0);

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, ';');

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, '}');

		Lexer_Free(lexer);
	}

	{
		struct Lexer *lexer = NULL;
		struct Token token;

		const char input_string[] = ""
		"/* \"this is a comment, not string\" */\n"
		"  \"this is a string\" \n"
		"\n";

		lexer = Lexer_New();
		Lexer_SetInputString(lexer, input_string);

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, TK_STRING_LITERAL);
		TEST_STR(token.value.string, "this is a string");

		Lexer_Free(lexer);
	}

	{
		struct Lexer *lexer = NULL;
		struct Token token;

		const char input_string[] = ""
		"  123  3.14  .124 2.34e+5  1.33e-2 .0 0. 3e3\n"
		"\n";

		lexer = Lexer_New();
		Lexer_SetInputString(lexer, input_string);

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, TK_NUMBER);
		TEST_DOUBLE(token.value.number, 123);

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, TK_NUMBER);
		TEST_DOUBLE(token.value.number, 3.14);

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, TK_NUMBER);
		TEST_DOUBLE(token.value.number, .124);

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, TK_NUMBER);
		TEST_DOUBLE(token.value.number, 2.34e+5);

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, TK_NUMBER);
		TEST_DOUBLE(token.value.number, 1.33e-2);

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, TK_NUMBER);
		TEST_DOUBLE(token.value.number, 0.);

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, TK_NUMBER);
		TEST_DOUBLE(token.value.number, 0.);

		Lexer_NextToken(lexer, &token);
		TEST_INT(token.tag, TK_NUMBER);
		TEST_DOUBLE(token.value.number, 3e+3);

		Lexer_Free(lexer);
	}

	printf("%s: %d/%d/%d: (FAIL/PASS/TOTAL)\n", __FILE__,
		Test_GetFailCount(), Test_GetPassCount(), Test_GetTotalCount());

	return 0;
}

