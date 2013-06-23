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
#include <string.h>

typedef struct Parser Parser;
typedef struct Symbol Symbol;
typedef struct AstNode Node;
typedef struct Token Token;

static Node * program(Parser *parser);

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

static void put_back_token(Parser *parser)
{
  parser->curr_token_index =
      (parser->curr_token_index + TOKEN_BUF_SIZE - 1) % TOKEN_BUF_SIZE;
  parser->is_head_token = 0;
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

/* -------------------------------------------------------------------------- */
static struct SymbolTable *symbol_table(const Parser *parser)
{
  return parser->table;
}

static const Token *get_current_token(const Parser *parser)
{
  return &parser->token_buf[parser->curr_token_index];
}

static int token_tag(const Parser *parser) {
  const Token *token = get_current_token(parser);
  return token->tag;
}

static DataType token_data_type(const Parser *parser) {
  const Token *token = get_current_token(parser);
  return token->data_type;
}

static const char *token_name(const Parser *parser) {
  const Token *token = get_current_token(parser);
  return token->value.name;
}

static double token_number(const Parser *parser) {
  const Token *token = get_current_token(parser);
  return token->value.number;
}

static const char *token_string(const Parser *parser) {
  const Token *token = get_current_token(parser);
  return token->value.string;
}

static int get_next_token(Parser *parser)
{
  for (;;) {
    Token *token = NULL;
    parser->curr_token_index = (parser->curr_token_index + 1) % TOKEN_BUF_SIZE;
    token = &parser->token_buf[parser->curr_token_index];

    if (parser->is_head_token) {
      Lexer_NextToken(parser->lexer, token);
    }
    parser->is_head_token = 1;

    switch (token_tag(parser)) {
    case TK_COMMENT:
      continue;
    default:
      break;
    }

    break;
  }

  return token_tag(parser);
}

static void skip_until(Parser *parser, int until_tag)
{
  int tag = token_tag(parser);

  for (;;) {
    if (tag == until_tag) {
      break;
    }
    tag = get_next_token(parser);
  }
}

static int expect(Parser *parser, int expected_token_tag)
{
  if (get_next_token(parser) == expected_token_tag) {
    return 1;
  } else {
    put_back_token(parser);
    return 0;
  }
}

static int peek_next_token(Parser *parser)
{
  const int tag = get_next_token(parser);

  put_back_token(parser);

  return tag;
}

static int assert_next_token(Parser *parser, int expected_token_tag)
{
  const int tag = get_next_token(parser);

  if (tag != expected_token_tag) {
    fprintf(stderr, "compiler implementation error\n");
    abort();
  }
  return tag;
}

static Symbol *new_symbol(Parser *parser, int symbol_type)
{
  Symbol *symbol = SymbolTable_Add(
      symbol_table(parser),
      token_name(parser),
      symbol_type);

  if (symbol == NULL) {
    return NULL;
  }

  return symbol;
}

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

static DataType type_specifier(Parser *parser)
{
  DataType data_type = TYPE_NONE;

  get_next_token(parser);
  data_type = token_data_type(parser);

  if (data_type == TYPE_NONE) {
    put_back_token(parser);
    return TYPE_NONE;
  }

  return data_type;
}

/*
primary_expression
  ;
*/
static Node * primary_expression(Parser *parser)
{
  Symbol *symbol = NULL;
  Node *node = NULL;

  switch (get_next_token(parser)) {
  case TK_STRING:
    symbol = SymbolTable_Add(
        symbol_table(parser),
        token_string(parser),
        SYM_STRING_LITERAL);
    node = AstNode_New(NODE_STRING_LITERAL);
    node->value.symbol = symbol;
    break;
  default:
    break;
  }

  return node;
}

/*
return_statement
  : "return"
  : "return" expression
  ;
*/
static Node *return_statement(Parser *parser)
{
  Node *stmt_return = NULL;

  assert_next_token(parser, TK_KW_RETURN);

  /* TODO TMP */
  if (!expect(parser, TK_NUMBER)) {
    /* TODO error handling */
    skip_until(parser, ';');
    return stmt_return;
  }

  stmt_return = AstNode_New(NODE_RETURN_STMT);
  stmt_return->left = AstNode_New(NODE_EXPR);
  stmt_return->left->value.number = token_number(parser);

  return stmt_return;
}

/*
vardump_statement
  : "vardump" identifier
  ;
*/
static Node *vardump_statement(Parser *parser)
{
  Symbol *symbol = NULL;
  Node *stmt = NULL;

  assert_next_token(parser, TK_KW_VARDUMP);

  if (!expect(parser, TK_IDENTIFIER)) {
    parse_error(parser, "missing identifier after 'vardump'");
    skip_until(parser, ';');
    return NULL;
  }

  symbol = SymbolTable_Lookup(
      symbol_table(parser),
      token_name(parser));

  if (symbol == NULL) {
    parse_error(parser, "undefined variable specified for 'vardump'");
    skip_until(parser, ';');
    return NULL;
  }

  stmt = AstNode_New(NODE_VARDUMP);
  stmt->value.symbol = symbol;

  return stmt;
}

/*
function_call
  : identifier "(" arguments ")"
  ;
*/
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

/*
assignment_or_function_call
  : assignment
  | function_call
  ;
*/
static Node * assignment_or_function_call(Parser *parser)
{
  Node *node = NULL;
  Symbol *symbol = NULL;

  assert_next_token(parser, TK_IDENTIFIER);

  symbol = SymbolTable_Lookup(
      symbol_table(parser),
      token_name(parser));

  /* TODO TMP */
  {
    if (strcmp(token_name(parser), "print") == 0) {
      symbol = SymbolTable_Add(
          symbol_table(parser),
          token_name(parser),
          SYM_FUNCTION);
    }
  }

  switch (peek_next_token(parser)) {
  case '(':
    symbol->type = SYM_FUNCTION;
    node = function_call(parser);
    node->value.symbol = symbol;
    break;
  case '=':
    /* TODO TMP */
    assert_next_token(parser, '=');
    if (expect(parser, TK_NUMBER)) {
      node = AstNode_New(NODE_ASSIGN);
      node->value.symbol = symbol;
      node->left = AstNode_New(NODE_EXPR);
      node->left->value.number = token_number(parser);
    }

    break;
  default:
    parse_error(parser, "unexpected charactors");
    break;
  }

  return node;
}

/*
variable_declaration_list
  : "var" identifier ":" type_specifier initializer
  ;
*/
static Node *variable_declaration(Parser *parser)
{
  Symbol *symbol = NULL;
  Node *init_expr = NULL;
  Node *node = NULL;

  if (!expect(parser, TK_KW_VAR)) {
    return NULL;
  }

  if (!expect(parser, TK_IDENTIFIER)) {
    parse_error(parser, "missing variable name after 'var'");
    skip_until(parser, ';');
    return NULL;
  }
  symbol = new_symbol(parser, SYM_VAR);

  if (!expect(parser, ':')) {
    parse_error(parser, "missing ':' after variable name");
    skip_until(parser, ';');
    return NULL;
  }

  symbol->data_type = type_specifier(parser);
  if (symbol->data_type == TYPE_NONE) {
    parse_error(parser, "missing type name after ':'");
    skip_until(parser, ';');
    return NULL;
  }

  if (!expect(parser, ';')) {
    parse_error(parser, "missing ';' at the end");
    return NULL;
  }

  node = AstNode_New(NODE_VAR_DECL);
  node->value.symbol = symbol;
  node->left = init_expr;

  return node;
}

/*
variable_declaration_list
  :
  | variable_declaration variable_declaration_list
  ;
*/
static Node *variable_declaration_list(Parser *parser)
{
  Node *decl = variable_declaration(parser);

  if (decl == NULL) {
    return NULL;
  }

  return list_node(decl, variable_declaration_list(parser));
}

/*
statement
  : assignment_or_function_call
  | return_statement
  | vardump_statement
  ;
*/
static Node *statement(Parser *parser)
{
  Node *stmt = NULL;

  switch (peek_next_token(parser)) {

  case TK_KW_RETURN:
    stmt = return_statement(parser);
    break;

  case TK_KW_VARDUMP:
    stmt = vardump_statement(parser);
    break;

  case TK_IDENTIFIER:
    stmt = assignment_or_function_call(parser);
    break;

  default:
    return NULL;
  }

  if (!expect(parser, ';')) {
    parse_error(parser, "missing ';'");
    skip_until(parser, ';');
  }

  return stmt;
}

/*
statement_list
  : statement statement_list
  ;
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
function_body
  : "{" variable_declaration_list statement_list "}"
  ;
*/
static Node *function_body(Parser *parser)
{
  Node *decl_list = NULL;
  Node *stmt_list = NULL;

  if (!expect(parser, '{')) {
    parse_error(parser, "missing '{'");
    skip_until(parser, ';');
  }

  decl_list = variable_declaration_list(parser);
  stmt_list = statement_list(parser);

  if (!expect(parser, '}')) {
    parse_error(parser, "missing '}'");
  }

  return block_node(decl_list, stmt_list);
}

/*
function_signature
  : ':' type_specifier '(' parameter_list ')'
  ;
*/
static Node *function_signature(Parser *parser)
{
  Node *param_list = NULL;

  if (!expect(parser, ':')) {
    parse_error(parser, "missing ':' after function name");
    skip_until(parser, ')');
    return NULL;
  }

  parser->return_type = type_specifier(parser);

  if (!expect(parser, '(')) {
    parse_error(parser, "missing '(' after return type");
    skip_until(parser, ')');
    return NULL;
  }

  if (!expect(parser, ')')) {
    parse_error(parser, "missing ')' after parameter list");
    skip_until(parser, ')');
    return NULL;
  }

  return param_list;
}

/*
function_definition
  : "function" identifier function_signature function_body
  ;
*/
static Node *function_definition(Parser *parser)
{
  Node *func_def = NULL;
  Symbol *symbol = NULL;

  assert_next_token(parser, TK_KW_FUNCTION);

  if (!expect(parser, TK_IDENTIFIER)) {
    parse_error(parser, "missing function name after 'function'");
    skip_until(parser, ':');
    put_back_token(parser); /* put back ':' for function_signature */
  }

  symbol = SymbolTable_Add(
      symbol_table(parser),
      token_name(parser),
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
program
  : function_definition
  ;
*/
static Node *program(Parser *parser)
{
  switch (peek_next_token(parser)) {

  case TK_KW_FUNCTION:
    return function_definition(parser);

  default:
    break;
  }

  return NULL;
}

