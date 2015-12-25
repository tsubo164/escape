/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "parser.h"
#include "memory.h"
#include <assert.h>
#include <string.h>

typedef struct ast_node node_t;
typedef struct symbol symbol_t;
typedef struct token token_t;
typedef struct parser parser_t;

static void syntax_error(parser_t *p, const char *msg)
{
  fprintf(stderr, "syntax error: %d, %s\n",
        lex_get_line_num(&p->lex), msg);
  exit(1);
}

static const token_t *get_token(parser_t *p)
{
  return lex_get_token(&p->lex);
}

static const token_t *unget_token(parser_t *p)
{
  return lex_unget_token(&p->lex);
}

static const token_t *current_token(parser_t *p)
{
  return lex_current_token(&p->lex);
}

static int assert_next(parser_t *p, int kind)
{
  const token_t *tok = get_token(p);
  if (kind_of(tok) == kind) {
    return 1;
  }
  else {
    fprintf(stderr, "compiler implementation error, ");
    fprintf(stderr, "assert '%s' but got '%s'.\n",
        kind_to_string(kind), kind_to_string(tok->kind));
    exit(1);
  }
  return 0;
}

static int expect(parser_t *p, int kind)
{
  const token_t *tok = get_token(p);
  if (kind_of(tok) == kind) {
    return 1;
  } else {
    fprintf(stderr, "syntak error: %d, expected '%s' but got '%s' [%s].\n",
        lex_get_line_num(&p->lex),
        kind_to_string(kind),
        kind_to_string(tok->kind),
        word_value_of(tok));
    exit(1);
    return 0;
  }
}

static int next(parser_t *p, int kind)
{
  const token_t *tok = get_token(p);
  if (kind_of(tok) == kind) {
    return 1;
  } else {
    unget_token(p);
    return 0;
  }
}

static node_t *list_node(node_t *current, node_t *next)
{
  return new_node(AST_LIST, current, next);
}

node_t *ast_number(const char *number_string)
{
  node_t *n = new_node(AST_LITERAL, NULL, NULL);
  strcpy(n->value.word,number_string);
  return n;
}

node_t *ast_identifier(const char *number_string)
{
  node_t *n = new_node(AST_SYMBOL, NULL, NULL);
  strcpy(n->value.word,number_string);
  return n;
}

static symbol_t *make_symbol(parser_t *p)
{
  int kind = SYM_NONE;
  const token_t *tok = current_token(p);

  if (kind_of(tok) == TK_VAR) {
    kind = SYM_VAR;
  } else {
  }
  return add_symbol(p->symtbl, word_value_of(tok), kind);
}

static node_t *identifier(parser_t *p)
{
  node_t *id = NULL;
  if (!expect(p, TK_IDENTIFIER)) {
    return NULL;
  }
  id = new_node(AST_SYMBOL, NULL, NULL);
  id->value.symbol = make_symbol(p);
  return id;
}

node_t *string_literal(parser_t *p)
{
  const token_t *tok = NULL;
  node_t *sl = NULL;
  if (!expect(p, TK_STRING_LITERAL)) {
    return NULL;
  }
  tok = current_token(p);
  sl = new_node(AST_STRING_LITERAL, NULL, NULL);
  sl->value.symbol = add_symbol(p->symtbl, string_value_of(tok), SYM_STRING_LITERAL);
  return sl;
}

/* TODO ----------------------------------------------------------------- */
#if 0
static void parse_error(parser_t *p, const char *detail)
{
  ErrorInfo *info = NULL;

  if (p->error_count >= Parser_GetMaxErrorInfo(p)) {
    p->error_count++;
    return;
  }

  info = &p->errors[p->error_count];
  info->line_number = Lexer_GetLineNumber(p->lexer);
  info->detail = detail;

  p->error_count++;
}
#endif

/* -------------------------------------------------------------------------- */
#if 0
static struct SymbolTable *symbol_table(const parser_t *p)
{
  return p->table;
}

static const Token *get_current_token(const parser_t *p)
{
  return &p->token_buf[p->curr_token_index];
}

static int token_tag(const parser_t *p) {
  const Token *token = get_current_token(p);
  return token->tag;
}

static DataType token_data_type(const parser_t *p) {
  const Token *token = get_current_token(p);
  return token->data_type;
}

static const char *token_name(const parser_t *p) {
  const Token *token = get_current_token(p);
  return token->value.name;
}

static double token_integer_value(const parser_t *p) {
  const Token *token = get_current_token(p);
  return token->value.Integer;
}

static double token_float_value(const parser_t *p) {
  const Token *token = get_current_token(p);
  return token->value.Float;
}

static const char *token_string(const parser_t *p) {
  const Token *token = get_current_token(p);
  return token->value.string;
}

static int get_next_token(parser_t *p)
{
  for (;;) {
    Token *token = NULL;
    p->curr_token_index = (p->curr_token_index + 1) % TOKEN_BUF_SIZE;
    token = &p->token_buf[p->curr_token_index];

    if (p->is_head_token) {
      Lexer_NextToken(p->lexer, token);
    }
    p->is_head_token = 1;

#if 0
    switch (token_tag(p)) {
    /*
    case TK_COMMENT:
      continue;
    */
    default:
      break;
    }
#endif

    break;
  }

  return token_tag(p);
}
#endif

#if 0
static void skip_until(parser_t *p, int until_tag)
{
  while (kind_of(get_token(p)) != until_tag) {
    /* do nothing */
  }
}
#endif

static int peek_token(parser_t *p)
{
  const token_t *tok = get_token(p);
  const int kind = tok->kind;
  unget_token(p);
  return kind;
}

/* to avoid too many recursive calls for just simple list like statement_list */
typedef struct node_list {
  node_t *head;
  node_t *tail;
} node_list_t;
#define INIT_NODE_LIST {NULL, NULL}

static node_t *append(node_list_t *list, node_t *node)
{
  if (node == NULL) {
    return NULL;
  }

  if (list->tail == NULL) {
    /* the first append */
    list->tail = list_node(node, NULL);
    list->head = list->tail;
  } else {
    list->tail->rnode = list_node(node, NULL);
    list->tail = list->tail->rnode;
  }
  return node;
}

static int type_specifier(parser_t *p)
{
  int type = TYPE_UNKNOWN;
  const token_t *tok = get_token(p);

  switch (kind_of(tok)) {
  case TK_BOOL:
    type = TYPE_BOOL;
    break;
  case TK_CHAR:
    type = TYPE_CHAR;
    break;
  case TK_SHORT:
    type = TYPE_SHORT;
    break;
  case TK_INT:
    type = TYPE_INT;
    break;
  case TK_LONG:
    type = TYPE_LONG;
    break;
  case TK_FLOAT:
    type = TYPE_FLOAT;
    break;
  case TK_DOUBLE:
    type = TYPE_DOUBLE;
    break;
  case TK_STRING:
    type = TYPE_STRING;
    break;
  default:
    unget_token(p);
    break;
  }

  return type;
}

/* PROTOTYPES */
static node_t *statement(parser_t *p);
static node_t *expression(parser_t *p);

/*
primary_expression
  : TK_INT_LITERAL
  | TK_FLOAT_LITERAL
  | TK_IDENTIFIER
  | '(' expression ')'
  ;
*/
static node_t *primary_expression(parser_t *p)
{
  /*
  Symbol *symbol = NULL;
  */
  node_t *node = NULL;
  const token_t *tok = get_token(p);

  switch (kind_of(tok)) {

  case TK_NUMBER:
    return ast_number(word_value_of(tok));

  case TK_IDENTIFIER:
#if 0
    {
      /* TODO TMP */
      if (strcmp(word_value_of(tok), "print") == 0) {
        symbol = SymbolTable_Add(
            symbol_table(p),
            token_name(p),
            SYM_FUNCTION);
      }
    }
    symbol = SymbolTable_Lookup(
        symbol_table(p),
        token_name(p));
    if (symbol == NULL) {
      /* TODO error handling */
    }
#endif
    unget_token(p);
    return identifier(p);

  case TK_STRING_LITERAL:
    unget_token(p);
    return string_literal(p);

  case '(':
    node = expression(p);
    if (!expect(p, ')')) {
    }
    break;

  default:
    syntax_error(p, "uexpected token");
    unget_token(p);
    break;
  }

  return node;
}

/*
argument_expression_list
  ;
*/
static node_t *argument_expression_list(parser_t *p)
{
  /* TODO TEST ONLY ONE ARGUMENT */
  return expression(p);
}

/*
postfix_expression
  : primary_expression
  | postfix_expression TK_INC
  | postfix_expression TK_DEC
  | ...
  ;
*/
static node_t *postfix_expression(parser_t *p)
{
  node_t *node = primary_expression(p);
  if (next(p, TK_INC)) {
    node = new_node(AST_POST_INC, node, NULL);
  }
  else if (next(p, TK_DEC)) {
    node = new_node(AST_POST_DEC, node, NULL);
  }
  /* TODO TEST */
  else if (next(p, '(')) {
    node_t *args = argument_expression_list(p);
    if (!expect(p, ')')) {
    }
    node = new_node(AST_CALL_EXPR, node, args);
  }
  /* TODO END OF TEST */
  return node;
}

/*
unary_expression
  : postfix_expression
  | TK_INC unary_expression
  | TK_DEC unary_expression
  | ...
  ;
*/
static node_t *unary_expression(parser_t *p)
{
  node_t *node = NULL;
  if (next(p, TK_INC)) {
    node = new_node(AST_PRE_INC, NULL, unary_expression(p));
  }
  else if (next(p, TK_DEC)) {
    node = new_node(AST_PRE_DEC, NULL, unary_expression(p));
  } else {
    node = postfix_expression(p);
  }
  return node;
}

/*
multiplicative_expression
  : unary_expression
  | multiplicative_expression '*' unary_expression
  | multiplicative_expression '/' unary_expression
  ;
*/
static node_t *multiplicative_expression(parser_t *p)
{
  node_t *node = unary_expression(p);
  for (;;) {
    int new_op = AST_NUL;
    if      (next(p, '*')) { new_op = AST_MUL; }
    else if (next(p, '/')) { new_op = AST_DIV; }
    else if (next(p, '%')) { new_op = AST_MOD; }
    else { break; }
    node = new_node(new_op, node, unary_expression(p));
  }
  return node;
}

/*
additive_expression
  : multiplicative_expression
  | additive_expression '+' multiplicative_expression
  | additive_expression '-' multiplicative_expression
  ;
*/
static node_t *additive_expression(parser_t *p)
{
  node_t *node = multiplicative_expression(p);
  for (;;) {
    int new_op = AST_NUL;
    if      (next(p, '+')) { new_op = AST_ADD; }
    else if (next(p, '-')) { new_op = AST_SUB; }
    else { break; }
    node = new_node(new_op, node, multiplicative_expression(p));
  }
  return node;
}

/*
shift_expression
  : additive_expression
  | shift_expression TK_LSHIFT additive_expression
  | shift_expression TK_RSHIFT additive_expression
  ;
*/
static node_t *shift_expression(parser_t *p)
{
  node_t *node = additive_expression(p);
  for (;;) {
    int new_op = AST_NUL;
    if      (next(p, TK_LSHIFT)) { new_op = AST_LSHIFT; }
    else if (next(p, TK_RSHIFT)) { new_op = AST_RSHIFT; }
    else { break; }
    node = new_node(new_op, node, additive_expression(p));
  }
  return node;
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
static node_t *relational_expression(parser_t *p)
{
  node_t *node = shift_expression(p);
  for (;;) {
    int new_op = AST_NUL;
    if      (next(p, '<'))   { new_op = AST_LT; }
    else if (next(p, '>'))   { new_op = AST_GT; }
    else if (next(p, TK_LE)) { new_op = AST_LE; }
    else if (next(p, TK_GE)) { new_op = AST_GE; }
    else { break; }
    node = new_node(new_op, node, shift_expression(p));
  }
  return node;
}

/*
equality_expression
  : relational_expression
  | equality_expression TK_EQ relational_expression
  | equality_expression TK_NE relational_expression
  ;
*/
static node_t *equality_expression(parser_t *p)
{
  node_t *node = relational_expression(p);
  for (;;) {
    int new_op = AST_NUL;
    if      (next(p, TK_EQ))   { new_op = AST_EQ; }
    else if (next(p, TK_NE))   { new_op = AST_NE; }
    else { break; }
    node = new_node(new_op, node, relational_expression(p));
  }
  return node;
}

/*
bitwise_and_expression
  : equality_expression
  | bitwise_and_expression '&' equality_expression
  ;
*/
static node_t *bitwise_and_expression(parser_t *p)
{
  node_t *node = equality_expression(p);
  for (;;) {
    int new_op = AST_NUL;
    if (next(p, '&')) { new_op = AST_BITWISE_AND; }
    else { break; }
    node = new_node(new_op, node, equality_expression(p));
  }
  return node;
}

/*
bitwise_xor_expression
  : bitwise_and_expression
  | bitwise_xor_expression '^' bitwise_and_expression
  ;
*/
static node_t *bitwise_xor_expression(parser_t *p)
{
  node_t *node = bitwise_and_expression(p);
  for (;;) {
    int new_op = AST_NUL;
    if (next(p, '^')) { new_op = AST_BITWISE_XOR; }
    else { break; }
    node = new_node(new_op, node, bitwise_and_expression(p));
  }
  return node;
}

/*
bitwise_or_expression
  : bitwise_xor_expression
  | bitwise_or_expression '|' bitwise_xor_expression
  ;
*/
static node_t *bitwise_or_expression(parser_t *p)
{
  node_t *node = bitwise_xor_expression(p);
  for (;;) {
    int new_op = AST_NUL;
    if (next(p, '|')) { new_op = AST_BITWISE_OR; }
    else { break; }
    node = new_node(new_op, node, bitwise_xor_expression(p));
  }
  return node;
}

/*
logical_and_expression
  : bitwise_or_expression
  | logical_and_expression TK_AND bitwise_or_expression
  ;
*/
static node_t *logical_and_expression(parser_t *p)
{
  node_t *node = bitwise_or_expression(p);
  for (;;) {
    int new_op = AST_NUL;
    if (next(p, TK_AND)) { new_op = AST_AND; }
    else { break; }
    node = new_node(new_op, node, bitwise_or_expression(p));
  }
  return node;
}

/*
logical_or_expression
  : logical_and_expression
  | logical_or_expression TK_OR logical_and_expression
  ;
*/
static node_t *logical_or_expression(parser_t *p)
{
  node_t *node = logical_and_expression(p);
  for (;;) {
    int new_op = AST_NUL;
    if (next(p, TK_OR)) { new_op = AST_OR; }
    else { break; }
    node = new_node(new_op, node, logical_and_expression(p));
  }
  return node;
}

/*
conditional_expression
  : logical_or_expression
  | logical_or_expression '?' expression ':' conditional_expression
  ;
*/
static node_t *conditional_expression(parser_t *p)
{
  return logical_or_expression(p);
}

/*
assignment_expression
  : conditional_expression
  | unary_expression '=' assignment_expression
  ;
*/
static node_t *assignment_expression(parser_t *p)
{
  node_t *node = conditional_expression(p);
  for (;;) {
    if (next(p, '=')) {
      node = new_node(AST_ASSIGN, node, assignment_expression(p));
    } else {
      break;
    }
  }
  return node;
}

/*
expression
  : assignment_expression
  | expression ',' assignment_expression
  ;
*/
static node_t *expression(parser_t *p)
{
  /*
  node_t *node = conditional_expression(p);
  */
  node_t *node = assignment_expression(p);
  return node;
}

/*
expression_statement
  : ';'
  | expression ';'
  ;
*/
static node_t *expression_statement(parser_t *p)
{
  node_t *expr = NULL;

  if (next(p, ';')) {
    return new_node(AST_EXPR_STMT, NULL, NULL);
  }
 
  expr = expression(p);
  if (!expect(p, ';')) {
  }
  return new_node(AST_EXPR_STMT, expr, NULL);
}

/*
empty_statement
  : ';'
  ;
*/
static node_t *empty_statement(parser_t *p)
{
  assert_next(p, ';');
  return new_node(AST_EMPTY_STMT, NULL, NULL);
}

/*
vardump_statement
  : "vardump" identifier
  ;
*/
static node_t *vardump_statement(parser_t *p)
{
  node_t *idnt = NULL;

  assert_next(p, TK_VARDUMP);
  idnt = identifier(p);
  if (!expect(p, ';')) {
  }
  return new_node(AST_VARDUMP, idnt, NULL);
}

/*
goto_statement
  : TK_GOTO identifier ';'
  ;
*/
static node_t *goto_statement(parser_t *p)
{
  node_t *idnt = NULL;

  assert_next(p, TK_GOTO);
  idnt = identifier(p);
  if (!expect(p, ';')) {
  }
  return new_node(AST_GOTO, idnt, NULL);
}

/*
label_statement
  : TK_LABEL identifier ':' statement
  ;
*/
static node_t *label_statement(parser_t *p)
{
  node_t *stmt = NULL;
  node_t *idnt = NULL;

  assert_next(p, TK_LABEL);
  idnt = identifier(p);
  if (!expect(p, ':')) {
  }
  stmt = statement(p);
  if (stmt == NULL) {
    syntax_error(p, "labeled with no statement");
  }
  return new_node(AST_LABEL, idnt, stmt);
}

/*
case_statement
  : TK_CASE expression ':' statement
  ;
*/
static node_t *case_statement(parser_t *p)
{
  node_t *expr = NULL;
  node_t *stmt = NULL;

  assert_next(p, TK_CASE);
  expr = expression(p);
  if (expr == NULL) {
    syntax_error(p, "missing expression");
  }
  if (!expect(p, ':')) {
  }
  stmt = statement(p);
  if (stmt == NULL) {
    syntax_error(p, "case labeled with no statement");
  }
  return new_node(AST_CASE, expr, stmt);
}

/*
default_statement
  : TK_CASE expression ':' statement
  ;
*/
static node_t *default_statement(parser_t *p)
{
  node_t *stmt = NULL;

  assert_next(p, TK_DEFAULT);
  if (!expect(p, ':')) {
  }
  stmt = statement(p);
  if (stmt == NULL) {
    syntax_error(p, "default labeled with no statement");
  }
  return new_node(AST_DEFAULT, stmt, NULL);
}

/*
variable_declaration
  : TK_VAR identifier type_specifier ';'
  | TK_VAR identifier type_specifier initializer ';'
  | TK_VAR identifier initializer ';'
  ;
*/
static node_t *variable_declaration(parser_t *p)
{
  node_t *expr = NULL;
  node_t *idnt = NULL;

  assert_next(p, TK_VAR);
  idnt = identifier(p);
  idnt->value.symbol->type = type_specifier(p);

  if (next(p, '=')) {
    expr = expression(p);
  }
  if (expr == NULL) {
    expr = new_node(AST_LITERAL, NULL, NULL);
    expr->value.word[0] = '0';
    expr->value.word[1] = '\0';
  }

  if (!expect(p, ';')) {
  }
  return new_node(AST_VAR_DECL, idnt, expr);
}

/*
variable_declaration_list
  : variable_declaration 
  | variable_declaration variable_declaration_list
  ;
*/
#if 0
static node_t *variable_declaration_list(parser_t *p)
{
  node_t *decl = variable_declaration(p);

  if (decl == NULL) {
    return NULL;
  }

  return list_node(decl, variable_declaration_list(p));
}
#endif

/*
statement_list
  : statement
  | statement statement_list
  ;
*/
static node_t *statement_list(parser_t *p)
{
  node_list_t list = INIT_NODE_LIST;
  for (;;) {
    node_t *stmt = statement(p);
    if (stmt == NULL) { break; }
    append(&list, stmt);
  }
  return list.head;
}

/*
block_statement
  : '{' variable_declaration_list statement_list '}'
  ;
*/
static node_t *block_statement(parser_t *p)
{
  node_t *stmt_list = NULL;

  if (!expect(p, '{')) {
  }
  stmt_list = statement_list(p);

  if (!expect(p, '}')) {
  }
  return new_node(AST_COMPOUND, stmt_list, NULL);
}

/*
break_statement
  : TK_BREAK ';'
  ;
*/
static node_t *break_statement(parser_t *p)
{
  node_t *stmt = NULL;

  assert_next(p, TK_BREAK);
  stmt = new_node(AST_BREAK, NULL, NULL);
  if (!expect(p, ';')) {
  }
  return stmt;
}

/*
continue_statement
  : TK_CONTINUE ';'
  ;
*/
static node_t *continue_statement(parser_t *p)
{
  node_t *stmt = NULL;

  assert_next(p, TK_CONTINUE);
  stmt = new_node(AST_CONTINUE, NULL, NULL);
  if (!expect(p, ';')) {
  }
  return stmt;
}

/*
return_statement
  : TK_RETURN ';'
  | TK_RETURN expression ';'
  ;
*/
static node_t *return_statement(parser_t *p)
{
  node_t *stmt = NULL;

  assert_next(p, TK_RETURN);
  stmt = new_node(AST_RETURN, NULL, NULL);
  if (next(p, ';')) {
    return stmt;
  }

  stmt->lnode = expression(p);
  if (!expect(p, ';')) {
  }
  return stmt;
}

/*
if_statement
  : TK_IF '(' expression ')' statement
  | TK_IF '(' expression ')' statement TK_ELSE statement
  ;
*/
static node_t *if_statement(parser_t *p)
{
  node_t *expr = NULL;
  node_t *then = NULL;

  assert_next(p, TK_IF);

  if (!expect(p, '(')) {
  }
  expr = expression(p);

  if (!expect(p, ')')) {
  }

  then = new_node(AST_THEN, statement(p), NULL);

  if (next(p, TK_ELSE)) {
    then->rnode = statement(p);
  }

  return new_node(AST_IF, expr, then);
}

#if 0
/*
case_clause
  : TK_CASE expression ':' statement
  | TK_DEFAULT ':' statement
  ;
*/
static node_t *case_clause(parser_t *p)
{
#if 0
  NodeList list = INIT_NODE_LIST;
  node_t *expr = NULL;

  if (!expect(p, TK_CASE) && !expect(p, TK_DEFAULT)) {
    /* TODO ABORT? */
    return NULL;
  }

  if (token_tag(p) == TK_CASE) {
    expr = expression(p);
  }

  if (!expect(p, ':')) {
    parse_error(p, "missing ':' at the end of case tag");
    skip_until(p, ';');
  }

  for (;;) {
    node_t *stmt = NULL;
    const int next = peek_next_token(p);

    if (next == TK_CASE || next == TK_DEFAULT) {
      break;
    }

    stmt = statement(p);
    if (stmt == NULL) {
      break;
    }

    append(&list, stmt);
  }

  return new_node(NODE_CASE_STMT, expr, list.head);
#endif
  return NULL;
}

/*
case_clause_list
  : case_clause case_clause_list
  ;
*/
static node_t *case_clause_list(parser_t *p)
{
  node_t *clause = case_clause(p);
  if (clause == NULL) {
    return NULL;
  }
  return list_node(clause, case_clause_list(p));
#if 0
  node_t *list = NULL;
  node_t *tail = NULL;
  node_t *clause = NULL;

  clause = case_clause(p);
  if (clause == NULL) {
    return NULL;
  }
  list = list_node(clause, NULL);

  for (;;) {
    clause = case_clause(p);
    if (clause == NULL) {
      break;
    }
    tail->rnode = list_node(clause, NULL);
    tail = tail->rnode;
  }
  return list;
#endif
#if 0
  NodeList list = INIT_NODE_LIST;

  for (;;) {
    const int next = peek_next_token(p);

    if (next != TK_CASE && next != TK_DEFAULT) {
      break;
    }

    append(&list, case_clause(p));
  }

  return list.head;
#endif
  return NULL;
}
#endif

/*
switch_statement
  : TK_SWITCH '(' expression ')' statement
  ;
*/
static node_t *switch_statement(parser_t *p)
{
  node_t *expr = NULL;
  node_t *stmt = NULL;

  assert_next(p, TK_SWITCH);
  if (!expect(p, '(')) {
  }

  expr = expression(p);
  if (!expect(p, ')')) {
  }

  stmt = statement(p);
  if (stmt == NULL) {
    syntax_error(p, "missing statement");
  }

  return new_node(AST_SWITCH, expr, stmt);
}

/*
for_statement
  : TK_FOR '(' statement expression ';' statement ')' statement
  ;
*/
static node_t *for_statement(parser_t *p)
{
  node_t *init = NULL;
  node_t *cond = NULL;
  node_t *expr = NULL;
  node_t *body = NULL;
  node_t *iter = NULL;

  assert_next(p, TK_FOR);

  if (!expect(p, '(')) {
  }

  if (peek_token(p) == TK_VAR) {
    init = statement(p);
  } else {
    init = expression_statement(p);
  }
  if (init == NULL) {
    syntax_error(p, "missing statement");
  }

  expr = expression_statement(p);
  if (expr == NULL) {
    syntax_error(p, "missing statement");
  }

  if (peek_token(p) != ')') {
    iter = expression(p);
  }
  if (!expect(p, ')')) {
  }

  body = new_node(AST_FOR_BODY, iter, statement(p));
  cond = new_node(AST_FOR_COND, expr, body);

  return new_node(AST_FOR_INIT, init, cond);
}

/*
while_statement
  : TK_WHILE '(' expression ')' statement
  ;
*/
static node_t *while_statement(parser_t *p)
{
  node_t *expr = NULL;
  node_t *stmt = NULL;

  assert_next(p, TK_WHILE);
  if (!expect(p, '(')) {
  }
  expr = expression(p);
  if (!expect(p, ')')) {
  }

  stmt = statement(p);
  return new_node(AST_WHILE, expr, stmt);
}

/*
do_while_statement
  : TK_DO statement '(' expression ')' TK_WHILE
  ;
*/
static node_t *do_while_statement(parser_t *p)
{
  node_t *expr = NULL;
  node_t *stmt = NULL;

  assert_next(p, TK_DO);

  stmt = statement(p);
  if (!expect(p, TK_WHILE)) {
  }

  if (!expect(p, '(')) {
  }
  expr = expression(p);
  if (!expect(p, ')')) {
  }

  if (!expect(p, ';')) {
  }

  return new_node(AST_DO_WHILE, stmt, expr);
}

/*
statement
  : assignment_or_function_call
  | return_statement
  | vardump_statement
  | jump_statement
  ;
*/
static node_t *statement(parser_t *p)
{
  switch (peek_token(p)) {

  /* selection statements */
  case TK_IF:
    return if_statement(p);
  case TK_SWITCH:
    return switch_statement(p);

  /* iteration statements */
  case TK_FOR:
    return for_statement(p);
  case TK_WHILE:
    return while_statement(p);
  case TK_DO:
    return do_while_statement(p);

  /* jump statements */
  case TK_BREAK:
    return break_statement(p);
  case TK_CONTINUE:
    return continue_statement(p);
  case TK_GOTO:
    return goto_statement(p);
  case TK_RETURN:
    return return_statement(p);

  /* declaration statements */
  case TK_VAR:
    return variable_declaration(p);

  /* labeled statements */
  case TK_LABEL:
    return label_statement(p);
  case TK_CASE:
    return case_statement(p);
  case TK_DEFAULT:
    return default_statement(p);

  /* expression statements */
  case TK_IDENTIFIER:
  case TK_INC:
  case TK_DEC:
    return expression_statement(p);

  /* builtin operators */
  case TK_VARDUMP:
    return vardump_statement(p);

  case ';':
    return empty_statement(p);

  case '{':
    return block_statement(p);

  default:
    return NULL;
  }
}

/*
function_body
  : '{' variable_declaration_list statement_list '}'
  ;
*/
/* TODO should use block_statement? */
static node_t *function_body(parser_t *p)
{
  return block_statement(p);
}

/*
function_parameters
  : '(' parameter_list ')'
  ;
*/
static node_t *function_parameters(parser_t *p)
{
  node_t *param_list = NULL;
  if (!expect(p, '(')) {
  }
  if (!expect(p, ')')) {
  }
  return param_list;
}

/*
function_definition
  : TK_FUNCTION TK_IDENTIFIER ':' type_specifier function_parameters function_body
  ;
*/
static node_t *function_definition(parser_t *p)
{
  char buf[128] = {'\0'};
  const token_t *tok = NULL;
  node_t *func_def = NULL;

  assert_next(p, TK_FN);
  if (!expect(p, TK_IDENTIFIER)) {
  }
  tok = current_token(p);
  strcpy(buf, word_value_of(tok));

  func_def = new_node(AST_FN_DEF, NULL, NULL);
  strcpy(func_def->value.word, buf);
  func_def->lnode = function_parameters(p);

  if (!expect(p, TK_INT)) {
  }

  func_def->rnode = function_body(p);
  return func_def;
}

/*
external_declaration
  : function_definition
  | variable_declaration
  ;
*/
static node_t *external_declaration(parser_t *p)
{
  switch (peek_token(p)) {
  case TK_FN:  return function_definition(p);
  case TK_VAR: return variable_declaration(p);
  case TK_EOS: return NULL;
  default:
    printf("[%s]\n", kind_to_string(kind_of(current_token(p))));
    syntax_error(p, "unexpected declaration");
    return NULL;
  }
}

/*
external_declaration_list
  : function_parameters
  | function_parameters
  ;
*/
static node_t *external_declaration_list(parser_t *p)
{
  node_list_t list = INIT_NODE_LIST;
  for (;;) {
    node_t *decl = external_declaration(p);
    if (decl == NULL) { break; }
    append(&list, decl);
  }
  return list.head;
}

/* TODO ----------------------------------------------------------------- */

/*
program
  : statement_list
  ;
*/
static node_t *program(parser_t *p)
{
  return external_declaration_list(p);
  /*
  return function_definition(p);
  return statement_list(p);
  */
}

struct ast_node *parse_file(struct parser *p, const char *filename)
{
  lex_input_file(&p->lex, filename);
  return program(p);
}

void parse_finish(struct parser *p)
{
  lex_finish(&p->lex);
}

/* oldsrc */
#if 0
#include "p.h"
#include "datatype.h"
#include "memory.h"
#include "symbol.h"
#include "lexer.h"
#include "ast.h"

#include <stdarg.h>
#include <string.h>

typedef struct parser_t parser_t;
typedef struct Symbol Symbol;
typedef struct AstNode node_t;
typedef struct Token Token;

typedef struct ErrorInfo ErrorInfo;
enum { TOKEN_BUF_SIZE = 2 };
enum { MAX_ERROR_INFO = 5 };

struct parser_t {
  struct Lexer *lexer;
  struct SymbolTable *table;

  struct Token token_buf[TOKEN_BUF_SIZE];
  int is_head_token;
  int curr_token_index;

  /* TODO TMP */
  ErrorInfo errors[MAX_ERROR_INFO];
  int error_count;
};

struct parser_t *Parser_New(void)
{
  struct parser_t *p = NULL;
  struct Lexer *lexer = NULL;

  p = MEMORY_ALLOC(struct parser_t);
  if (p == NULL) {
    /* TODO error handling */
    return NULL;
  }

  lexer = Lexer_New();
  if (lexer == NULL) {
    /* TODO error handling */
    return NULL;
  }

  p->lexer = lexer;

  p->is_head_token = 1;
  p->curr_token_index = 0;

  p->error_count = 0;

  return p;
}

void Parser_Free(struct parser_t *p)
{
  if (p == NULL) {
    return;
  }
  Lexer_Free(p->lexer);
  MEMORY_FREE(p);
}

struct AstNode *Parser_ParseFile(struct parser_t *p,
    FILE *input_file, struct SymbolTable *table)
{
  Lexer_SetInputFile(p->lexer, input_file);
  p->table = table;

  return program(p);
}

struct AstNode *Parser_ParseString(struct parser_t *p,
    const char *input_string, struct SymbolTable *table)
{
  Lexer_SetInputString(p->lexer, input_string);
  p->table = table;

  return program(p);
}

int Parser_GetErrorCount(const struct parser_t *p)
{
  return p->error_count;
}

int Parser_GetMaxErrorInfo(const struct parser_t *p)
{
  return MAX_ERROR_INFO;
}

const struct ErrorInfo *Parser_GetErrorInfo(struct parser_t *p, int index)
{
  if (Parser_GetErrorCount(p) == 0) {
    return NULL;
  }
  if (index > Parser_GetMaxErrorInfo(p)) {
    return NULL;
  }

  return &p->errors[index];
}

#endif
