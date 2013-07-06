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

static Node *program(Parser *parser);
static Node *expression(Parser *parser);
static Node *statement(Parser *parser);

typedef struct ErrorInfo ErrorInfo;
enum { TOKEN_BUF_SIZE = 2 };
enum { MAX_ERROR_INFO = 5 };

struct Parser {
  struct Lexer *lexer;
  struct SymbolTable *table;

  struct Token token_buf[TOKEN_BUF_SIZE];
  int is_head_token;
  int curr_token_index;

  /* TODO TMP */
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
  while (get_next_token(parser) != until_tag) {
    /* do nothing */
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
  if (get_next_token(parser) != expected_token_tag) {
    fprintf(stderr, "compiler implementation error\n");
    abort();
  }
  return token_tag(parser);
}

static Node *new_node(int op, Node *lhs, Node *rhs)
{
  Node *node = AstNode_New(op);
  node->left  = lhs;
  node->right = rhs;
  return node;
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
  : TK_NUMBER
  | TK_IDENTIFIER
  | '(' expression ')'
  ;
*/
static Node *primary_expression(Parser *parser)
{
  Symbol *symbol = NULL;
  Node *node = NULL;

  switch (get_next_token(parser)) {

  case TK_NUMBER:
    node = AstNode_New(NODE_NUMBER_LITERAL);
    node->value.number = token_number(parser);
    break;

  case TK_IDENTIFIER:
    symbol = SymbolTable_Lookup(
        symbol_table(parser),
        token_name(parser));
    if (symbol == NULL) {
      /* TODO error handling */
    }
    node = AstNode_New(NODE_SYMBOL);
    node->value.symbol = symbol;
    break;

  case '(':
    node = expression(parser);
    if (!expect(parser, ')')) {
      parse_error(parser, "missing ')' in expression"/*, token_string(parser)*/);
    }
    break;

  default:
    parse_error(parser, "unexpected token");
    put_back_token(parser);
    node = AstNode_New(NODE_NULL);
    break;
  }

  return node;
}

/*
multiplicative_expression
  : primary_expression
  | multiplicative_expression '*' primary_expression
  | multiplicative_expression '/' primary_expression
  ;
*/
static Node *multiplicative_expression(Parser *parser)
{
  Node *root = primary_expression(parser);

  while (expect(parser, '*') || expect(parser, '/')) {
    const int new_op = token_tag(parser) == '*' ? NODE_MUL : NODE_DIV;
    root = new_node(new_op, root, primary_expression(parser));
  }

  return root;
}

/*
additive_expression
  : multiplicative_expression
  | additive_expression '+' multiplicative_expression
  | additive_expression '-' multiplicative_expression
  ;
*/
static Node *additive_expression(Parser *parser)
{
  Node *root = multiplicative_expression(parser);

  while (expect(parser, '+') || expect(parser, '-')) {
    const int new_op = token_tag(parser) == '+' ? NODE_ADD : NODE_SUB;
    root = new_node(new_op, root, multiplicative_expression(parser));
  }

  return root;
}

/*
shift_expression
  : additive_expression
  | shift_expression TK_LSHIFT additive_expression
  | shift_expression TK_RSHIFT additive_expression
  ;
*/
static Node *shift_expression(Parser *parser)
{
  Node *root = additive_expression(parser);

  while (expect(parser, TK_LSHIFT) || expect(parser, TK_RSHIFT)) {
    const int new_op = token_tag(parser) == TK_LSHIFT ? NODE_LSHIFT : NODE_RSHIFT;
    root = new_node(new_op, root, additive_expression(parser));
  }

  return root;
}

/*
relational_expression
  : shift_expression
  | relational_expression '<' shift_expression
  | relational_expression '>' shift_expression
  | relational_expression TK_LE shift_expression
  | relational_expression TK_GE shift_expression
  ;
*/
static Node *relational_expression(Parser *parser)
{
  Node *root = shift_expression(parser);

  while (expect(parser, '<')   ||
         expect(parser, '>')   ||
         expect(parser, TK_LE) ||
         expect(parser, TK_GE)) {
    int new_op = NODE_NULL;
    switch (token_tag(parser)) {
    case '<':   new_op = NODE_LT; break;
    case '>':   new_op = NODE_GT; break;
    case TK_LE: new_op = NODE_LE; break;
    case TK_GE: new_op = NODE_GE; break;
    }
    root = new_node(new_op, root, shift_expression(parser));
  }

  return root;
}

/*
equality_expression
  : relational_expression
  | equality_expression TK_EQ relational_expression
  | equality_expression TK_NE relational_expression
  ;
*/
static Node *equality_expression(Parser *parser)
{
  Node *root = relational_expression(parser);

  while (expect(parser, TK_EQ) || expect(parser, TK_NE)) {
    const int new_op = token_tag(parser) == TK_EQ ? NODE_EQ : NODE_NE;
    root = new_node(new_op, root, relational_expression(parser));
  }

  return root;
}

/*
bitwise_and_expression
  : equality_expression
  | bitwise_and_expression '&' equality_expression
  ;
*/
static Node *bitwise_and_expression(Parser *parser)
{
  Node *root = equality_expression(parser);

  while (expect(parser, '&')) {
    root = new_node(NODE_BITWISE_AND, root, equality_expression(parser));
  }

  return root;
}

/*
bitwise_xor_expression
  : bitwise_and_expression
  | bitwise_xor_expression '^' bitwise_and_expression
  ;
*/
static Node *bitwise_xor_expression(Parser *parser)
{
  Node *root = bitwise_and_expression(parser);

  while (expect(parser, '^')) {
    root = new_node(NODE_BITWISE_XOR, root, bitwise_and_expression(parser));
  }

  return root;
}

/*
bitwise_or_expression
  : bitwise_xor_expression
  | bitwise_or_expression '|' bitwise_xor_expression
  ;
*/
static Node *bitwise_or_expression(Parser *parser)
{
  Node *root = bitwise_xor_expression(parser);

  while (expect(parser, '|')) {
    root = new_node(NODE_BITWISE_OR, root, bitwise_xor_expression(parser));
  }

  return root;
}

/*
logical_and_expression
  : bitwise_or_expression
  | logical_and_expression TK_AND bitwise_or_expression
  ;
*/
static Node *logical_and_expression(Parser *parser)
{
  Node *root = bitwise_or_expression(parser);

  while (expect(parser, TK_AND)) {
    root = new_node(NODE_AND, root, bitwise_or_expression(parser));
  }

  return root;
}

/*
logical_or_expression
  : logical_and_expression
  | logical_or_expression TK_OR logical_and_expression
  ;
*/
static Node *logical_or_expression(Parser *parser)
{
  Node *root = logical_and_expression(parser);

  while (expect(parser, TK_OR)) {
    root = new_node(NODE_OR, root, logical_and_expression(parser));
  }

  return root;
}

/*
expression
  : logical_or_expression
  ;
*/
static Node *expression(Parser *parser)
{
  Node *node = logical_or_expression(parser);

  return node;
}

/*
argument_list
  ;
*/
static Node * argument_list(Parser *parser)
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
  : TK_KW_RETURN
  | TK_KW_RETURN expression
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

  if (!expect(parser, ';')) {
    parse_error(parser, "missing ';' at the end of return statement");
    skip_until(parser, ';');
  }

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
    return AstNode_New(NODE_NULL);
  }

  symbol = SymbolTable_Lookup(
      symbol_table(parser),
      token_name(parser));

  if (symbol == NULL) {
    parse_error(parser, "undefined variable specified for 'vardump'");
    skip_until(parser, ';');
    return AstNode_New(NODE_NULL);
  }

  stmt = AstNode_New(NODE_VARDUMP);
  stmt->value.symbol = symbol;

  if (!expect(parser, ';')) {
    parse_error(parser, "missing ';' at the end of varialbe declaration");
    skip_until(parser, ';');
  }

  return stmt;
}

/*
function_call
  : TK_IDENTIFIER "(" arguments_list ")"
  ;
*/
static Node * function_call(Parser *parser)
{
  Node *node = NULL;
  Node *args = NULL;

  assert_next_token(parser, '(');

  args = argument_list(parser);

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
    if (!expect(parser, ';')) {
      parse_error(parser, "missing ';' at the end of function call");
      skip_until(parser, ';');
    }
    break;

  case '=':
    /* TODO TMP */
    assert_next_token(parser, '=');
    node = AstNode_New(NODE_ASSIGN);
    node->value.symbol = symbol;
    node->left = expression(parser);
    if (!expect(parser, ';')) {
      parse_error(parser, "missing ';' at the end of assignment");
      skip_until(parser, ';');
    }
    break;

  default:
    parse_error(parser, "unexpected charactors");
    break;
  }

  return node;
}

/*
variable_declaration
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

  if (peek_next_token(parser) == '=') {
    get_next_token(parser); /* read '=' */
    init_expr = expression(parser);
  }

  if (!expect(parser, ';')) {
    parse_error(parser, "missing ';' at the end");
    return NULL;
  }

  node = AstNode_New(NODE_VAR_DECL);
  node->value.symbol = symbol;
  node->right = init_expr;

  return node;
}

/*
variable_declaration_list
  : variable_declaration 
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
statement_list
  : statement
  | statement statement_list
  ;
*/
static Node *statement_list(Parser *parser)
{
  Node *stmt = statement(parser);

  if (stmt == NULL) {
    return NULL;
  }

  return list_node(stmt, statement_list(parser));
}

/*
block_statement
  : '{' variable_declaration_list statement_list '}'
  ;
*/
static Node *block_statement(Parser *parser)
{
  Node *decl_list = NULL;
  Node *stmt_list = NULL;
  Node *block = NULL;

  if (!expect(parser, '{')) {
    parse_error(parser, "missing '{'");
    skip_until(parser, ';');
  }

  decl_list = variable_declaration_list(parser);
  stmt_list = statement_list(parser);

  if (!expect(parser, '}')) {
    parse_error(parser, "missing '}'");
  }

  block = AstNode_New(NODE_BLOCK);
  block->left  = decl_list;
  block->right = stmt_list;
  return block;
}

#if 0
static Node *else_clause(Parser *parser)
{
  if (!expect(parser, TK_KW_ELSE)) {
    return NULL;
  }

  return statement(parser);
}

/*
else_clause_list
  : TK_KW_IF '(' expression ')' block_statement else_clause_list
  ;
*/
static Node *else_clause_list(Parser *parser)
{
  Node *stmt = else_clause(parser);

  if (stmt == NULL) {
    return NULL;
  }

  return list_node(stmt, else_clause_list(parser));
#if 0
  while (expect(parser, TK_KW_ELSE)) {
    if (peek_next_token(parser) != '{') {
      parse_error(parser, "missing '{' after 'else'");
      skip_until(parser, '}');
    }
 
    if (peek_next_token(parser) != TK_KW_IF) {
      block = if_statement(parser);
    } else {
      block = block_statement(parser);
    }
  }
#endif
}
#endif

/*
if_statement
  : TK_KW_IF '(' expression ')' block_statement else_clause_list
  ;
*/
static Node *if_statement(Parser *parser)
{
  Node *expr = NULL;
  Node *cond = NULL;
  Node *next = NULL;

  assert_next_token(parser, TK_KW_IF);

  if (!expect(parser, '(')) {
    parse_error(parser, "missing '(' after 'if'");
    skip_until(parser, ')');
  }

  expr = expression(parser);

  if (!expect(parser, ')')) {
    parse_error(parser, "missing ')' after conditional expression");
    skip_until(parser, '{');
    put_back_token(parser);
  }

  cond = new_node(NODE_COND, expr, statement(parser));

  if (expect(parser, TK_KW_ELSE)) {
    next = statement(parser);
  }

  return new_node(NODE_IF, cond, next);
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

  case TK_KW_IF:
    stmt = if_statement(parser);
    break;

  case TK_KW_RETURN:
    stmt = return_statement(parser);
    break;

  case TK_KW_VARDUMP:
    stmt = vardump_statement(parser);
    break;

  case TK_IDENTIFIER:
    stmt = assignment_or_function_call(parser);
    break;

  case '{':
    stmt = block_statement(parser);
    break;

  default:
    return NULL;
  }

  return stmt;
}

/*
function_body
  : '{' variable_declaration_list statement_list '}'
  ;
*/
/* TODO should use block_statement? */
static Node *function_body(Parser *parser)
{
  return block_statement(parser);
}

/*
function_parameters
  : '(' parameter_list ')'
  ;
*/
static Node *function_parameters(Parser *parser)
{
  Node *param_list = NULL;

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
  : TK_FUNCTION TK_IDENTIFIER ':' type_specifier function_parameters function_body
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
    put_back_token(parser); /* put back ':' for function_parameters */
  }

  symbol = SymbolTable_Add(
      symbol_table(parser),
      token_name(parser),
      SYM_FUNCTION);

  if (symbol == NULL) {
    parse_error(parser, "function already defined");
    return NULL;
  }

  if (!expect(parser, ':')) {
    parse_error(parser, "missing ':' after function name");
    skip_until(parser, '(');
    return NULL;
  }

  symbol->data_type = type_specifier(parser);

  func_def = AstNode_New(NODE_FUNC_DEF);
  func_def->left  = function_parameters(parser);
  func_def->right = function_body(parser);
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

