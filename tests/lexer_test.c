#include "lexer.h"
#include "unit_test.h"
#include <stdio.h>

int main()
{
  const char filename[] = "lexer_test.es";
  {
    FILE *file = fopen(filename, "w");
    const char src[] =
        " fn if for while else 123 .23 \n  1.2312e+3\n \t 23.23f \v 82."
        "  +  ++  - -- \n"
        " || | && &\n"
        "  ! = != ==  \n"
        "  > >= >> \n"
        "  < <= << \n"
        "  / //  \n + "
        "  += /*  //   this is comment \n */  + \n"
        "  -= /*  //   this is comment \n */  - \n"
        "  *= /*  //   this is comment \n */  * \n"
        "  /= /*  //   this is comment \n */  / \n"
        "    hoge  int \n"
        "\n";
    fprintf(file, "%s", src);
    fclose(file);
  }
	{
		struct lexer l = LEXER_INIT;
		struct token *tok;

		lex_input_file(&l, filename);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_FN);

		TEST_INT(lex_get_line_num(&l), 1);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_IF);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_FOR);

		TEST_INT(lex_get_column_num(&l), 10);
		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_WHILE);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_ELSE);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_INT_LITERAL);
		TEST_INT(int_value_of(tok), 123);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_FLOAT_LITERAL);
		TEST_FLOAT(float_value_of(tok), .23);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_FLOAT_LITERAL);
		TEST_FLOAT(float_value_of(tok), 1.2312e+3);

		TEST_INT(lex_get_line_num(&l), 2);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_FLOAT_LITERAL);
		TEST_FLOAT(float_value_of(tok), 23.23);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_FLOAT_LITERAL);
		TEST_FLOAT(float_value_of(tok), 82.);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), '+');

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_INC);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), '-');

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_DEC);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_OR);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), '|');

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_AND);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), '&');

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), '!');

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), '=');

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_NE);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_EQ);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), '>');

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_GE);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_RSHIFT);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), '<');

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_LE);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_LSHIFT);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), '/');

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), '+');

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_ADD_ASSIGN);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), '+');

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_SUB_ASSIGN);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), '-');

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_MUL_ASSIGN);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), '*');

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_DIV_ASSIGN);

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), '/');

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_IDENT);
		TEST_STR(word_value_of(tok), "hoge");

		tok = lex_get_token(&l);
		TEST_INT(kind_of(tok), TK_INT);

		lex_finish(&l);
	}
#if 0
	{
		struct lexer l = LEXER_INIT;
		struct token tok;

		const char input_string[] = ""
		"/* escape minimum program */\n"
		"\n"
		"fn main () int\n"
		"{\n"
		"  return 0;\n"
		"}\n";

		lex_input_string(&l, input_string);

		tok = lex_get_token(l);
		TEST_INT(tok.tag, TK_FN);

		Lexer_NextToken(l);
		TEST_INT(tok.tag, TK_IDENTIFIER);
		TEST_STR(tok.value.name, "main");

		Lexer_NextToken(l);
		TEST_INT(tok.tag, ':');

		Lexer_NextToken(l);
		TEST_INT(tok.tag, TK_INT);

		Lexer_NextToken(l);
		TEST_INT(tok.tag, '(');

		Lexer_NextToken(l);
		TEST_INT(tok.tag, ')');

		Lexer_NextToken(l);
		TEST_INT(tok.tag, '{');

		Lexer_NextToken(l);
		TEST_INT(tok.tag, TK_RETURN);

		Lexer_NextToken(l);
		TEST_INT(tok.tag, TK_INT_LITERAL);
		TEST_DOUBLE(tok.value.Integer, 0);

		Lexer_NextToken(l);
		TEST_INT(tok.tag, ';');

		Lexer_NextToken(l);
		TEST_INT(tok.tag, '}');

		lex_finish(&l);
	}
#endif
#if 0
	{
		struct Lexer *lexer = NULL;
		struct Token token;

		const char input_string[] = ""
		"/* \"this is a comment, not string\" */\n"
		"  \"this is a string\" \n"
		"\n";

		lexer = Lexer_New();
		Lexer_SetInputString(lexer, input_string);

		Lexer_NextToken(lexeren);
		TEST_INT(token.tag, TK_STRING_LITERAL);
		TEST_STR(token.value.string, "this is a string");

		Lexer_Free(lexer);
	}

	{
		struct Lexer *lexer = NULL;
		struct Token token;

		const char input_string[] = ""
		"  123  3.14  .124 2.34e+5  1.33e-2 .0 0. 3e3f  234230212321L \n"
		"\n";

		lexer = Lexer_New();
		Lexer_SetInputString(lexer, input_string);

		Lexer_NextToken(lexeren);
		TEST_INT(token.tag, TK_INT_LITERAL);
		TEST_DOUBLE(token.value.Integer, 123);

		Lexer_NextToken(lexeren);
		TEST_INT(token.tag, TK_FLOAT_LITERAL);
		TEST_DOUBLE(token.value.Float, 3.14);

		Lexer_NextToken(lexeren);
		TEST_INT(token.tag, TK_FLOAT_LITERAL);
		TEST_DOUBLE(token.value.Float, .124);

		Lexer_NextToken(lexeren);
		TEST_INT(token.tag, TK_FLOAT_LITERAL);
		TEST_DOUBLE(token.value.Float, 2.34e+5);

		Lexer_NextToken(lexeren);
		TEST_INT(token.tag, TK_FLOAT_LITERAL);
		TEST_DOUBLE(token.value.Float, 1.33e-2);

		Lexer_NextToken(lexeren);
		TEST_INT(token.tag, TK_FLOAT_LITERAL);
		TEST_DOUBLE(token.value.Float, 0.);

		Lexer_NextToken(lexeren);
		TEST_INT(token.tag, TK_FLOAT_LITERAL);
		TEST_DOUBLE(token.value.Float, 0.);

		Lexer_NextToken(lexeren);
		TEST_INT(token.tag, TK_FLOAT_LITERAL);
		TEST_DOUBLE(token.value.Float, 3e+3);

		Lexer_NextToken(lexeren);
		TEST_INT(token.tag, TK_INT_LITERAL);
		TEST_DOUBLE(token.value.Integer, 234230212321);

		Lexer_Free(lexer);
	}
	{
		struct Lexer *lexer = NULL;
		struct Token token;

		const char input_string[] = ""
		"   'c';  \n"
		"\n";

		lexer = Lexer_New();
		Lexer_SetInputString(lexer, input_string);

		Lexer_NextToken(lexeren);
		TEST_INT(token.tag, TK_INT_LITERAL);
		TEST_INT(token.value.Integer, 'c');

		Lexer_Free(lexer);
	}
#endif

	printf("%s: %d/%d/%d: (FAIL/PASS/TOTAL)\n", __FILE__,
		TestGetFailCount(), TestGetPassCount(), TestGetTotalCount());

	return 0;
}
