/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "parser.h"
#include "datatype.h"
#include "memory.h"
#include "symbol.h"
#include "lexer.h"
#include "ast.h"

#include <stdarg.h>

typedef struct Parser Parser;
typedef struct Symbol Symbol;
typedef struct AstNode Node;
typedef struct Token Token;

static Node * program                     (Parser *parser);

static Node * function_definition         (Parser *parser);
static Node * function_signature          (Parser *parser);
static Node * function_body               (Parser *parser);

static Node * statement_list              (Parser *parser);
static Node * statement                   (Parser *parser);

static Node * return_statement            (Parser *parser);
static Node * assignment_or_function_call (Parser *parser);
static Node * function_call               (Parser *parser);

static Node * primary_expression          (Parser *parser);

static DataType type_specifier            (Parser *parser);

static const Token *get_current_token(Parser *parser);
static const Token *get_next_token(Parser *parser);
static const Token *peek_next_token(Parser *parser);
static void put_back_token(Parser *parser);

/* TODO TEST */
#if 0
static int accept_next(Parser *parser, int token_tag);
#endif

static const Token *assert_next_token(Parser *parser, int expected_token_tag);
static void parse_error(Parser *parser, const char *detail);
static void skip_until(Parser *parser, int token_tag);

static Node *list_node(Node *current, Node *next)
{
	Node *node = AstNode_New(NODE_LIST);
	node->left  = current;
	node->right = next;
	return node;
}

static Node *block_node(Node *decl_list, Node *stmt_list)
{
	Node *node = AstNode_New(NODE_BLOCK);
	node->left  = decl_list;
	node->right = stmt_list;
	return node;
}

typedef struct ErrorInfo ErrorInfo;
enum { TOKEN_BUF_SIZE = 2 };
enum { MAX_ERROR_INFO = 5 };

struct Parser {
	struct Lexer *lexer;
	struct SymbolTable *table;

	struct Token token_buf[TOKEN_BUF_SIZE];
	int is_head_token;
	int curr_token_index;

	/* TODO */
	DataType return_type;
	ErrorInfo errors[MAX_ERROR_INFO];
	int error_count;
};

struct Parser *Parser_New(void)
{
	struct Parser *parser = NULL;
	struct Lexer *lexer = NULL;

	parser = MEMORY_ALLOC(struct Parser);
	if (parser == NULL) {
		/* TODO error handling */
		return NULL;
	}

	lexer = Lexer_New();
	if (lexer == NULL) {
		/* TODO error handling */
		return NULL;
	}

	parser->lexer = lexer;

	parser->is_head_token = 1;
	parser->curr_token_index = 0;

	parser->error_count = 0;

	return parser;
}

void Parser_Free(struct Parser *parser)
{
	if (parser == NULL) {
		return;
	}
	Lexer_Free(parser->lexer);
	MEMORY_FREE(parser);
}

struct AstNode *Parser_ParseFile(struct Parser *parser,
		FILE *input_file, struct SymbolTable *table)
{
	Lexer_SetInputFile(parser->lexer, input_file);
	parser->table = table;

	return program(parser);
}

Node *Parser_ParseString(struct Parser *parser,
		const char *input_string, struct SymbolTable *table)
{
	Lexer_SetInputString(parser->lexer, input_string);
	parser->table = table;

	return program(parser);
}

int Parser_GetErrorCount(const struct Parser *parser)
{
	return parser->error_count;
}

int Parser_GetMaxErrorInfo(const struct Parser *parser)
{
	return MAX_ERROR_INFO;
}

const struct ErrorInfo *Parser_GetErrorInfo(struct Parser *parser, int index)
{
	if (Parser_GetErrorCount(parser) == 0) {
		return NULL;
	}
	if (index > Parser_GetMaxErrorInfo(parser)) {
		return NULL;
	}

	return &parser->errors[index];
}

static Node *program(struct Parser *parser)
{
	const struct Token *token = NULL;

	token = peek_next_token(parser);
	switch (token->tag) {
	case TK_KW_FUNCTION:
		return function_definition(parser);
	default:
		break;
	}

	return NULL;
}

/*
function_definition
	: "function" identifier function_signature function_body
	;
*/
static Node *function_definition(struct Parser *parser)
{
	const Token *token = NULL;
	Node *func_def = NULL;
	Symbol *symbol = NULL;

	assert_next_token(parser, TK_KW_FUNCTION);

	token = get_next_token(parser);
	if (token->tag != TK_IDENTIFIER) {
		parse_error(parser, "missing function name after 'function'");
		skip_until(parser, ':');
		put_back_token(parser); /* put back ':' for function_signature */
	}

	symbol = SymbolTable_Add(
			parser->table,
			token->value.name,
			SYM_FUNCTION);

	func_def = AstNode_New(NODE_FUNC_DEF);
	func_def->left  = function_signature(parser);
	func_def->right = function_body(parser);

	/* TODO tmp */
	symbol->data_type = parser->return_type;

	func_def->value.symbol = symbol;

	return func_def;
}

/*
function_signature
	: ':' type_specifier '(' parameter_list ')'
*/
static Node *function_signature(struct Parser *parser)
{
	const Token *token = NULL;
	Node *param_list = NULL;

	token = get_next_token(parser);
	if (token->tag != ':') {
		parse_error(parser, "missing ':' after function name");
		goto error;
	}
/*

	if (!accept_next(parser, ':')) {
		parse_error(parser, "missing ':'");
		goto error;
	}
*/

	parser->return_type = type_specifier(parser);

	token = get_next_token(parser);
	if (token->tag != '(') {
		parse_error(parser, "missing '('");
		goto error;
	}

/*
	param_list = parameter_list(parser);
*/

	token = get_next_token(parser);
	if (token->tag != ')') {
		parse_error(parser, "missing ')'");
		goto error;
	}

	return param_list;

error:
	skip_until(parser, ')');
	return param_list;
}

static Node *function_body(struct Parser *parser)
{
	const Token *token = NULL;
	Node *decl_list = NULL;
	Node *stmt_list = NULL;

	token = get_next_token(parser);
	if (token->tag != '{') {
		parse_error(parser, "missing '{'");
		skip_until(parser, ';');
	}

	stmt_list = statement_list(parser);

	token = get_next_token(parser);
	if (token->tag != '}') {
		parse_error(parser, "missing '}'");
	}

	return block_node(decl_list, stmt_list);
}

/*
statement_list :=
	| statement statement_list
*/
static Node *statement_list(Parser *parser)
{
	Node *stmt = NULL;

	stmt = statement(parser);
	if (stmt == NULL) {
		return NULL;
	}

	return list_node(stmt, statement_list(parser));
}

/*
statement :=
	| return_statement ';'
*/
static Node *statement(Parser *parser)
{
	const Token *token = NULL;
	Node *stmt = NULL;

	token = peek_next_token(parser);
	switch (token->tag) {
	case TK_KW_RETURN:
		stmt = return_statement(parser);
		break;
	case TK_IDENTIFIER:
		stmt = assignment_or_function_call(parser);
		break;
	default:
		return NULL;
	}

	token = get_next_token(parser);
	if (token->tag != ';') {
		parse_error(parser, "missing ';'");
		skip_until(parser, ';');
	}

	return stmt;
}

static Node *return_statement(struct Parser *parser)
{
	const struct Token *token = NULL;
	Node *stmt_return = NULL;

	assert_next_token(parser, TK_KW_RETURN);

	token = get_next_token(parser);
	if (token->tag != TK_NUMBER) {
		/* TODO error handling */
		skip_until(parser, ';');
		return stmt_return;
	}

	stmt_return = AstNode_New(NODE_RETURN_STMT);
	stmt_return->left = AstNode_New(NODE_EXPR);
	stmt_return->left->value.number = token->value.number;

	return stmt_return;
}

/*
assignment_or_function_call
	: assignment
	| function_call
*/
static Node * assignment_or_function_call(Parser *parser)
{
	const struct Token *token = NULL;
	Node *node = NULL;
	Symbol *symbol = NULL;

	token = assert_next_token(parser, TK_IDENTIFIER);

	symbol = SymbolTable_Add(
			parser->table,
			token->value.name,
			SYM_FUNCTION);

	token = peek_next_token(parser);
	switch (token->tag) {
	case '(':
		symbol->type = SYM_FUNCTION;
		node = function_call(parser);
		node->value.symbol = symbol;
		break;
	default:
		parse_error(parser, "unexpected charactors");
		break;
	}

	return node;
}

static Node * function_call(Parser *parser)
{
	Node *node = NULL;
	Node *args = NULL;

	assert_next_token(parser, '(');

	args = primary_expression(parser);

	assert_next_token(parser, ')');

	node = AstNode_New(NODE_FUNC_CALL);
	node->left = args;

	return node;
}

static Node * primary_expression(Parser *parser)
{
	const Token *token = NULL;
	Symbol *symbol = NULL;
	Node *node = NULL;

	token = get_next_token(parser);
	switch (token->tag) {
	case TK_STRING:
		symbol = SymbolTable_Add(
				parser->table,
				token->value.string,
				SYM_STRING_LITERAL);
		node = AstNode_New(NODE_EXPR);
		node->value.symbol = symbol;
		break;
	default:
		break;
	}

	return node;
}

static DataType type_specifier(Parser *parser)
{
	const Token *token = NULL;

	token = get_next_token(parser);
	if (token->data_type == TYPE_NONE) {
		parse_error(parser, "missing type name");
		skip_until(parser, ';');
		return TYPE_NONE;
	}

	return token->data_type;
}

static const Token *get_current_token(Parser *parser)
{
	return &parser->token_buf[parser->curr_token_index];
}

static const Token *get_next_token(Parser *parser)
{
	Token *token = NULL;

	for (;;) {
		parser->curr_token_index = (parser->curr_token_index + 1) % TOKEN_BUF_SIZE;
		token = &parser->token_buf[parser->curr_token_index];

		if (parser->is_head_token) {
			Lexer_NextToken(parser->lexer, token);
		}
		parser->is_head_token = 1;

		switch (token->tag) {
		case TK_COMMENT:
			continue;
		default:
			break;
		}

		break;
	}

	return token;
}

static void put_back_token(Parser *parser)
{
	parser->curr_token_index =
			(parser->curr_token_index + TOKEN_BUF_SIZE - 1) % TOKEN_BUF_SIZE;
	parser->is_head_token = 0;
}

static const Token *peek_next_token(Parser *parser)
{
	const Token *token = get_next_token(parser);
	put_back_token(parser);

	return token;
}

/* TODO TEST */
#if 0
static int accept_next(Parser *parser, int token_tag)
{
	const Token *token = get_next_token(parser);

	if (token->tag != token_tag) {
		const Token *prev = NULL;
		put_back_token(parser);
		prev = get_current_token(parser);
		/*
		parse_error(parser, "missing function name after '%d'", token->tag);
		*/
		if (token->tag > 255) {
			printf("missing '%c' after '%d'\n", token_tag, prev->tag);
		} else {
			printf("missing '%c' after '%c'\n", token_tag, prev->tag);
		}
		get_next_token(parser);
		printf("HOGE---------\n");
		return 0;
	}

	return 1;
}
#endif

static const Token *assert_next_token(Parser *parser, int expected_token_tag)
{
	const Token *token = get_next_token(parser);
	if (token->tag != expected_token_tag) {
		fprintf(stderr, "compiler implementation error\n");
		abort();
	}
	return token;
}

static void parse_error(Parser *parser, const char *detail)
{
	ErrorInfo *info = NULL;

	if (parser->error_count >= Parser_GetMaxErrorInfo(parser)) {
		parser->error_count++;
		return;
	}

	info = &parser->errors[parser->error_count];
	info->line_number = Lexer_GetLineNumber(parser->lexer);
	info->detail = detail;

	parser->error_count++;
}

static void skip_until(Parser *parser, int token_tag)
{
	const Token *token = NULL;

	token = get_current_token(parser);
	for (;;) {
		if (token->tag == token_tag) {
			break;
		}
		token = get_next_token(parser);
	}
}

