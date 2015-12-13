/*
Copyright (c) 2012 Hiroshi Tsubokawa
See LICENSE and README
*/

#include "parser.h"
#include "memory.h"
#include <assert.h>
#include <string.h>

typedef struct ast_node node_t;
typedef struct token token_t;
typedef struct parser parser_t;

static const token_t *get_token(parser_t *p)
{
  return lex_get_token(&p->lex);
}

static const token_t *unget_token(parser_t *p)
{
  return lex_unget_token(&p->lex);
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
    fprintf(stderr, "syntak error, expected '%s' but got '%s'.\n",
        kind_to_string(kind), kind_to_string(tok->kind));
    exit(1);
    return 0;
  }
}

/*
static int is_next(parser_t *p, int kind)
{
  const token_t *tok = get_token(p);
  if (kind_of(tok) == kind) {
    printf("%s ", kind_to_string(tok->kind));
    return 1;
  } else {
    unget_token(p);
    return 0;
  }
}
*/

node_t *new_node(int kind)
{
  node_t *n = MEM_ALLOC(node_t);
  n->kind = kind;
  n->lnode = NULL;
  n->rnode = NULL;
  return n;
}

static node_t *list_node(node_t *current, node_t *next)
{
  node_t *node = new_node(AST_LIST);
  node->lnode = current;
  node->rnode = next;
  return node;
  /*
  return current->next = next;
  */
}

node_t *ast_number(const char *number_string)
{
  node_t *n = new_node(AST_LITERAL);
  strcpy(n->word,number_string);
  return n;
}

node_t *ast_var_decl(node_t *init)
{
  node_t *n = new_node(AST_VAR_DECL);
  n->init = init;
  return n;
}

/* TODO ----------------------------------------------------------------- */
static void parse_error(parser_t *p, const char *detail)
{
#if 0
  ErrorInfo *info = NULL;

  if (p->error_count >= Parser_GetMaxErrorInfo(p)) {
    p->error_count++;
    return;
  }

  info = &p->errors[p->error_count];
  info->line_number = Lexer_GetLineNumber(p->lexer);
  info->detail = detail;

  p->error_count++;
#endif
}

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

static void skip_until(parser_t *p, int until_tag)
{
  while (kind_of(get_token(p)) != until_tag) {
    /* do nothing */
  }
}

static int peek_token(parser_t *p)
{
  const token_t *tok = get_token(p);
  const int kind = tok->kind;
  unget_token(p);
  return kind;
}

#if 0
static Symbol *new_symbol(parser_t *p, int symbol_type)
{
  Symbol *symbol = SymbolTable_Add(
      symbol_table(p),
      token_name(p),
      symbol_type);

  if (symbol == NULL) {
    return NULL;
  }

  return symbol;
}
#endif

#if 0
/* TODO TEST */
typedef struct NodeList {
  node_t *head;
  node_t *tail;
} NodeList;
#define LIST_INIT {NULL, NULL}

static node_t *append(NodeList *list, node_t *node)
{
  if (node == NULL) {
    return NULL;
  }

  if (list->tail == NULL) {
    list->tail = list_node(node, NULL);
    list->head = list->tail;
  } else {
    list->tail->rnode = list_node(node, NULL);
    list->tail = list->tail->rnode;
  }
  return node;
}
#endif

#if 0
static DataType type_specifier(parser_t *p)
{
  DataType data_type = TYPE_NONE;

  get_next_token(p);
  data_type = token_data_type(p);

  if (data_type == TYPE_NONE) {
    put_back_token(p);
    return TYPE_NONE;
  }

  return data_type;
}
#endif

/*
argument_expression_list
  ;
*/
#if 0
static node_t *argument_expression_list(parser_t *p)
{
  Symbol *symbol = NULL;
  node_t *node = NULL;

  switch (peek_next_token(p)) {

  case TK_STRING_LITERAL:
    get_next_token(p);
    symbol = SymbolTable_Add(
        symbol_table(p),
        token_string(p),
        SYM_STRING_LITERAL);
    node = AstNode_New(NODE_STRING_LITERAL);
    node->value.symbol = symbol;
    break;

  default:
    break;
  }

  return node;
}
#endif

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
    return new_node(AST_NUL);
    break;

  case '(':
    node = expression(p);
    if (!expect(p, ')')) {
      parse_error(p, "missing ')' in expression"/*, token_string(p)*/);
    }
    break;

  default:
    parse_error(p, "unexpected token");
    unget_token(p);
    node = new_node(AST_NUL);
    break;
  }

  return node;
}

/*
postfix_expression
  : primary_expression
  | postfix_expression TK_INC
  | postfix_expression TK_DEC
  | postfix_expression '(' argument_expression_list ')'
  ;
*/
static node_t *postfix_expression(parser_t *p)
{
  node_t *root = primary_expression(p);
#if 0
  const token_t *tok = get_token(p);
  if (kind_of(tok) == TK_INC) {
  }
  else if (kind_of(tok) == TK_INC) {
  }
  if (expect(p, TK_INC) || expect(p, TK_DEC)) {
    const int new_op = kind_of(p) == TK_INC ? NODE_POST_INC : NODE_POST_DEC;
    root = new_node(new_op, root, NULL);
  }
  /* TODO TEST */
  else if (expect(p, '(')) {
    node_t *args = argument_expression_list(p);

    if (!expect(p, ')')) {
      parse_error(p, "missing ')' at the end of function call");
      skip_until(p, ';');
    }

    root = new_node(NODE_CALL_EXPR, root, args);
  }
#endif
  return root;
}

/*
unary_expression
  : postfix_expression
  | TK_INC unary_expression
  | TK_DEC unary_expression
  ;
*/
static node_t *unary_expression(parser_t *p)
{
  node_t *root = NULL;
#if 0
  if (expect(p, TK_INC) || expect(p, TK_DEC)) {
    const int new_op = token_tag(p) == TK_INC ? NODE_INC : NODE_DEC;
    root = new_node(new_op, root, unary_expression(p));
  } else {
    root = postfix_expression(p);
  }
#endif
  return root;
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
  node_t *root = unary_expression(p);
#if 0
  while (expect(p, '*') || expect(p, '/') || expect(p, '%')) {
    int new_op = NODE_NULL;
    switch (token_tag(p)) {
    case '*': new_op = NODE_MUL; break;
    case '/': new_op = NODE_DIV; break;
    case '%': new_op = NODE_MOD; break;
    }
    root = new_node(new_op, root, unary_expression(p));
  }
#endif
  return root;
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
  node_t *root = multiplicative_expression(p);
#if 0
  while (expect(p, '+') || expect(p, '-')) {
    const int new_op = token_tag(p) == '+' ? NODE_ADD : NODE_SUB;
    root = new_node(new_op, root, multiplicative_expression(p));
  }
#endif
  return root;
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
  node_t *root = additive_expression(p);
#if 0
  while (expect(p, TK_LSHIFT) || expect(p, TK_RSHIFT)) {
    const int new_op = token_tag(p) == TK_LSHIFT ? NODE_LSHIFT : NODE_RSHIFT;
    root = new_node(new_op, root, additive_expression(p));
  }
#endif
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
static node_t *relational_expression(parser_t *p)
{
  node_t *root = shift_expression(p);
#if 0
  while (expect(p, '<')   ||
         expect(p, '>')   ||
         expect(p, TK_LE) ||
         expect(p, TK_GE)) {
    int new_op = NODE_NULL;
    switch (token_tag(p)) {
    case '<':   new_op = NODE_LT; break;
    case '>':   new_op = NODE_GT; break;
    case TK_LE: new_op = NODE_LE; break;
    case TK_GE: new_op = NODE_GE; break;
    }
    root = new_node(new_op, root, shift_expression(p));
  }
#endif
  return root;
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
  node_t *root = relational_expression(p);
#if 0
  while (expect(p, TK_EQ) || expect(p, TK_NE)) {
    const int new_op = token_tag(p) == TK_EQ ? NODE_EQ : NODE_NE;
    root = new_node(new_op, root, relational_expression(p));
  }
#endif
  return root;
}

/*
bitwise_and_expression
  : equality_expression
  | bitwise_and_expression '&' equality_expression
  ;
*/
static node_t *bitwise_and_expression(parser_t *p)
{
  node_t *root = equality_expression(p);
#if 0
  while (expect(p, '&')) {
    root = new_node(NODE_BITWISE_AND, root, equality_expression(p));
  }
#endif
  return root;
}

/*
bitwise_xor_expression
  : bitwise_and_expression
  | bitwise_xor_expression '^' bitwise_and_expression
  ;
*/
static node_t *bitwise_xor_expression(parser_t *p)
{
  node_t *root = bitwise_and_expression(p);
#if 0
  while (expect(p, '^')) {
    root = new_node(NODE_BITWISE_XOR, root, bitwise_and_expression(p));
  }
#endif
  return root;
}

/*
bitwise_or_expression
  : bitwise_xor_expression
  | bitwise_or_expression '|' bitwise_xor_expression
  ;
*/
static node_t *bitwise_or_expression(parser_t *p)
{
  node_t *root = bitwise_xor_expression(p);
#if 0
  while (expect(p, '|')) {
    root = new_node(NODE_BITWISE_OR, root, bitwise_xor_expression(p));
  }
#endif
  return root;
}

/*
logical_and_expression
  : bitwise_or_expression
  | logical_and_expression TK_AND bitwise_or_expression
  ;
*/
static node_t *logical_and_expression(parser_t *p)
{
  node_t *root = bitwise_or_expression(p);
#if 0
  while (expect(p, TK_AND)) {
    root = new_node(NODE_AND, root, bitwise_or_expression(p));
  }
#endif
  return root;
}

/*
logical_or_expression
  : logical_and_expression
  | logical_or_expression TK_OR logical_and_expression
  ;
*/
static node_t *logical_or_expression(parser_t *p)
{
  node_t *root = logical_and_expression(p);
#if 0
  while (expect(p, TK_OR)) {
    root = new_node(NODE_OR, root, logical_and_expression(p));
  }
#endif
  return root;
}

/*
assignment_expression
  : conditional_expression
  | unary_expression '=' assignment_expression
  ;
*/
static node_t *assignment_expression(parser_t *p)
{
  node_t *lval = logical_or_expression(p);
#if 0
  if (expect(p, '=')) {
    return new_node(NODE_ASSIGN, lval, expression(p));
  }
#endif
  return lval;
}

/*
expression
  : logical_or_expression
  ;
*/
static node_t *expression(parser_t *p)
{
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
  node_t *expr = expression(p);
#if 0
  if (!expect(p, ';')) {
    parse_error(p, "missing ';' at the end of expression statement");
    skip_until(p, ';');
  }
  return new_node(NODE_EXPR_STMT, expr, NULL);
#endif
  return expr;
}

/*
null_statement
  : ';'
  ;
*/
static node_t *null_statement(parser_t *p)
{
  assert_next(p, ';');
  return new_node(AST_NUL);
}

/*
vardump_statement
  : "vardump" identifier
  ;
*/
static node_t *vardump_statement(parser_t *p)
{
  node_t *stmt = NULL;
#if 0
  Symbol *symbol = NULL;

  assert_next_token(p, TK_VARDUMP);

  if (!expect(p, TK_IDENTIFIER)) {
    parse_error(p, "missing identifier after 'vardump'");
    skip_until(p, ';');
    return AstNode_New(NODE_NULL);
  }

  symbol = SymbolTable_Lookup(
      symbol_table(p),
      token_name(p));

  if (symbol == NULL) {
    parse_error(p, "undefined variable specified for 'vardump'");
    skip_until(p, ';');
    return AstNode_New(NODE_NULL);
  }

  stmt = AstNode_New(NODE_VARDUMP);
  stmt->value.symbol = symbol;

  if (!expect(p, ';')) {
    parse_error(p, "missing ';' at the end of vardump statement");
    skip_until(p, ';');
  }
#endif
  return stmt;
}

/*
goto_statement
  : TK_GOTO identifier ';'
  ;
*/
static node_t *goto_statement(parser_t *p)
{
  node_t *symnode = NULL;
#if 0
  Symbol *symbol = NULL;

  assert_next_token(p, TK_GOTO);

  if (!expect(p, TK_IDENTIFIER)) {
    parse_error(p, "missing identifier after 'goto'");
    skip_until(p, ';');
    return AstNode_New(NODE_NULL);
  }

  symbol = SymbolTable_Add(
      symbol_table(p),
      token_name(p),
      SYM_LABEL);

  if (symbol == NULL) {
    parse_error(p, "undefined variable specified for 'goto'");
    skip_until(p, ';');
    return AstNode_New(NODE_NULL);
  }

  symnode = AstNode_New(NODE_SYMBOL);
  symnode->value.symbol = symbol;

  if (!expect(p, ';')) {
    parse_error(p, "missing ';' at the end of goto statement");
    skip_until(p, ';');
  }

  return new_node(NODE_GOTO_STMT, symnode, NULL);
#endif
  return symnode;
}

/*
labeled_statement
  : TK_LABEL identifier ':' statement
  ;
*/
static node_t *labeled_statement(parser_t *p)
{
  node_t *symnode = NULL;
#if 0
  Symbol *symbol = NULL;

  assert_next_token(p, TK_LABEL);

  if (!expect(p, TK_IDENTIFIER)) {
    parse_error(p, "missing identifier after 'label'");
    skip_until(p, ':');
    return AstNode_New(NODE_NULL);
  }

  symbol = SymbolTable_Add(
      symbol_table(p),
      token_name(p),
      SYM_LABEL);

  symnode = AstNode_New(NODE_SYMBOL);
  symnode->value.symbol = symbol;

  if (!expect(p, ':')) {
    parse_error(p, "missing ':' at the end of varialbe declaration");
    skip_until(p, ';');
  }

  return new_node(NODE_LABELED_STMT, symnode, statement(p));
#endif
  return symnode;
}

/*
variable_declaration
  : "var" identifier ":" type_specifier initializer
  ;
*/
static node_t *variable_declaration(parser_t *p)
{
  node_t *node = NULL;
#if 0
  node_t *init_expr = NULL;
  Symbol *symbol = NULL;

  if (!expect(p, TK_VAR)) {
    return NULL;
  }

  if (!expect(p, TK_IDENTIFIER)) {
    parse_error(p, "missing variable name after 'var'");
    skip_until(p, ';');
    return NULL;
  }
  symbol = new_symbol(p, SYM_VAR);

  if (!expect(p, ':')) {
    parse_error(p, "missing ':' after variable name");
    skip_until(p, ';');
    return NULL;
  }

  symbol->data_type = type_specifier(p);
  if (symbol->data_type == TYPE_NONE) {
    parse_error(p, "missing type name after ':'");
    skip_until(p, ';');
    return NULL;
  }

  if (peek_next_token(p) == '=') {
    get_next_token(p); /* read '=' */
    init_expr = expression(p);
  }

  if (!expect(p, ';')) {
    parse_error(p, "missing ';' at the end");
    return NULL;
  }

  node = AstNode_New(NODE_VAR_DECL);
  node->value.symbol = symbol;
  node->right = init_expr;
#endif
  return node;
}

/*
variable_declaration_list
  : variable_declaration 
  | variable_declaration variable_declaration_list
  ;
*/
static node_t *variable_declaration_list(parser_t *p)
{
  node_t *decl = variable_declaration(p);

  if (decl == NULL) {
    return NULL;
  }

  return list_node(decl, variable_declaration_list(p));
}

/*
statement_list
  : statement
  | statement statement_list
  ;
*/
static node_t *statement_list(parser_t *p)
{
  node_t *stmt = statement(p);
  if (stmt == NULL) {
    return NULL;
  }
  return list_node(stmt, statement_list(p));
}

/*
block_statement
  : '{' variable_declaration_list statement_list '}'
  ;
*/
static node_t *block_statement(parser_t *p)
{
  node_t *block = NULL;
#if 0
  node_t *decl_list = NULL;
  node_t *stmt_list = NULL;

  if (!expect(p, '{')) {
    parse_error(p, "missing '{'");
    skip_until(p, ';');
  }

  decl_list = variable_declaration_list(p);
  stmt_list = statement_list(p);

  if (!expect(p, '}')) {
    parse_error(p, "missing '}'");
  }

  block = AstNode_New(NODE_BLOCK);
  block->left  = decl_list;
  block->right = stmt_list;
#endif
  return block;
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
  stmt = new_node(AST_BREAK);
  if (!expect(p, ';')) {
    parse_error(p, "missing ';' at the end of break statement");
    skip_until(p, ';');
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
  stmt = new_node(AST_CONTINUE);
  if (!expect(p, ';')) {
    parse_error(p, "missing ';' at the end of continue statement");
    skip_until(p, ';');
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
  node_t *stmt_return = NULL;

  assert_next(p, TK_RETURN);
  stmt_return = new_node(AST_RETURN);
  if (expect(p, ';')) {
    return stmt_return;
  }
#if 0
  stmt_return->left = expression(p);

  if (!expect(p, ';')) {
    parse_error(p, "missing ';' at the end of return statement");
    skip_until(p, ';');
  }
#endif
  return stmt_return;
}

/*
if_statement
  : TK_IF '(' expression ')' statement
  | TK_IF '(' expression ')' statement TK_ELSE statement
  ;
*/
static node_t *if_statement(parser_t *p)
{
#if 0
  node_t *expr = NULL;
  node_t *cond = NULL;
  node_t *next = NULL;

  assert_next_token(p, TK_IF);

  if (!expect(p, '(')) {
    parse_error(p, "missing '(' after 'if'");
    skip_until(p, ')');
  }

  expr = expression(p);

  if (!expect(p, ')')) {
    parse_error(p, "missing ')' after conditional expression");
    skip_until(p, '{');
    put_back_token(p);
  }

  cond = new_node(NODE_COND, expr, statement(p));

  if (expect(p, TK_ELSE)) {
    next = statement(p);
  }

  return new_node(NODE_IF, cond, next);
#endif
  return NULL;
}

/*
case_clause
  : TK_CASE expression ':' statement
  | TK_DEFAULT ':' statement
  ;
*/
static node_t *case_clause(parser_t *p)
{
#if 0
  NodeList list = LIST_INIT;
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
#if 0
  NodeList list = LIST_INIT;

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

/*
switch_statement
  : TK_SWITCH '(' expression ')' '{' case_clause_list '}'
  ;
*/
static node_t *switch_statement(parser_t *p)
{
#if 0
  node_t *expr = NULL;
  node_t *case_list = NULL;

  assert_next_token(p, TK_SWITCH);

  if (!expect(p, '(')) {
    parse_error(p, "missing '(' after 'if'");
    skip_until(p, ')');
  }

  expr = expression(p);

  if (!expect(p, ')')) {
    parse_error(p, "missing ')' after conditional expression");
    skip_until(p, '{');
    put_back_token(p);
  }

  if (!expect(p, '{')) {
    parse_error(p, "missing '{' after 'switch' condition");
    skip_until(p, ';');
  }

  case_list = case_clause_list(p);

  if (!expect(p, '}')) {
    parse_error(p, "missing '}' after 'switch' case clauses");
    skip_until(p, ';');
  }

  return new_node(NODE_SWITCH, expr, case_list);
#endif
  return NULL;
}

/*
for_statement
  : TK_FOR '(' expression ';' expression ';' expression ')' statement
  ;
*/
static node_t *for_statement(parser_t *p)
{
#if 0
  node_t *init = NULL;
  node_t *cond = NULL;
  node_t *expr = NULL;
  node_t *loop = NULL;
  node_t *iter = NULL;

  assert_next_token(p, TK_FOR);

  if (!expect(p, '(')) {
    parse_error(p, "missing '(' after 'for'");
    skip_until(p, ')');
  }

  if (!expect(p, ';')) {
    init = expression(p);
    if (!expect(p, ';')) {
      parse_error(p, "missing ';' after the first 'for' expression");
      skip_until(p, ')');
    }
  }

  if (!expect(p, ';')) {
    expr = expression(p);
    if (!expect(p, ';')) {
      parse_error(p, "missing ';' after the second 'for' expression");
      skip_until(p, ')');
    }
  }

  if (!expect(p, ')')) {
    iter = expression(p);
    if (!expect(p, ')')) {
      parse_error(p, "missing ')' after the third 'for' expression");
      skip_until(p, '{');
      put_back_token(p);
    }
  }

  loop = new_node(NODE_FOR_LOOP, iter, statement(p));
  cond = new_node(NODE_FOR_COND, expr, loop);

  return new_node(NODE_FOR_INIT, init, cond);
#endif
  return NULL;
}

/*
while_statement
  : TK_WHILE '(' expression ')' statement
  ;
*/
static node_t *while_statement(parser_t *p)
{
#if 0
  node_t *expr = NULL;
  node_t *stmt = NULL;

  assert_next_token(p, TK_WHILE);

  if (!expect(p, '(')) {
    parse_error(p, "missing '(' after 'while'");
    skip_until(p, ')');
  }

  expr = expression(p);

  if (!expect(p, ')')) {
    parse_error(p, "missing ')' after conditional expression");
    skip_until(p, '{');
    put_back_token(p);
  }

  stmt = statement(p);

  return new_node(NODE_WHILE, expr, stmt);
#endif
  return NULL;
}

/*
do_while_statement
  : TK_DO statement '(' expression ')' TK_WHILE
  ;
*/
static node_t *do_while_statement(parser_t *p)
{
#if 0
  node_t *expr = NULL;
  node_t *stmt = NULL;

  assert_next_token(p, TK_DO);

  stmt = statement(p);

  if (!expect(p, TK_WHILE)) {
    parse_error(p, "missing 'while' after statements");
    skip_until(p, ';');
    put_back_token(p);
  }

  if (!expect(p, '(')) {
    parse_error(p, "missing '(' after 'do'");
    skip_until(p, ')');
  }

  expr = expression(p);

  if (!expect(p, ')')) {
    parse_error(p, "missing ')' after conditional expression");
    skip_until(p, ';');
    put_back_token(p);
  }

  if (!expect(p, ';')) {
    parse_error(p, "missing ';' at the end of 'do while' statement");
    skip_until(p, ';');
  }

  return new_node(NODE_DO_WHILE, stmt, expr);
#endif
  return NULL;
}

/*
jump_statement
  : TK_BREAK ';'
  | TK_CONTINUE ';'
  | TK_RETURN ';'
  ;
*/
static node_t *jump_statement(parser_t *p)
{
  switch (peek_token(p)) {
  case TK_BREAK:
    return break_statement(p);
  case TK_CONTINUE:
    return continue_statement(p);
  case TK_RETURN:
    return return_statement(p);
  default:
    return NULL;
  }
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
  node_t *stmt = NULL;

  switch (peek_token(p)) {

  /* selection statements */
  case TK_IF:
    stmt = if_statement(p);
    break;
  case TK_SWITCH:
    stmt = switch_statement(p);
    break;

  /* iteration statements */
  case TK_FOR:
    stmt = for_statement(p);
    break;
  case TK_WHILE:
    stmt = while_statement(p);
    break;
  case TK_DO:
    stmt = do_while_statement(p);
    break;

  case TK_BREAK:
  case TK_CONTINUE:
  case TK_RETURN:
    return jump_statement(p);

#if 0
    stmt = goto_statement(p);
    break;

  /* builtin operators */
  case TK_VARDUMP:
    stmt = vardump_statement(p);
    break;

  /* labeled statements */
  case TK_LABEL:
    stmt = labeled_statement(p);
    break;
#endif

  /*
  case TK_IDENTIFIER:
    stmt = assignment_or_function_call(p);
    break;
  */
  case TK_IDENTIFIER:
  case TK_INC:
  case TK_DEC:
    stmt = expression_statement(p);
    break;

  case ';':
    stmt = null_statement(p);
    break;

  case '{':
    stmt = block_statement(p);
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
    parse_error(p, "missing '(' after return type");
    skip_until(p, ')');
    return NULL;
  }

  if (!expect(p, ')')) {
    parse_error(p, "missing ')' after parameter list");
    skip_until(p, ')');
    return NULL;
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
#if 0
  node_t *func_def = NULL;
  Symbol *symbol = NULL;

  assert_next_token(p, TK_FUNCTION);

  if (!expect(p, TK_IDENTIFIER)) {
    parse_error(p, "missing function name after 'function'");
    skip_until(p, ':');
    put_back_token(p); /* put back ':' for function_parameters */
  }

  symbol = SymbolTable_Add(
      symbol_table(p),
      token_name(p),
      SYM_FUNCTION);

  if (symbol == NULL) {
    parse_error(p, "function already defined");
    return NULL;
  }

  if (!expect(p, ':')) {
    parse_error(p, "missing ':' after function name");
    skip_until(p, '(');
    return NULL;
  }

  symbol->data_type = type_specifier(p);

  func_def = AstNode_New(NODE_FUNC_DEF);
  func_def->left  = function_parameters(p);
  func_def->right = function_body(p);
  func_def->value.symbol = symbol;

  return func_def;
#endif
  return NULL;
}

/* TODO ----------------------------------------------------------------- */

#if 0
/*
variable_declaration
  : TK_VAR identifier type ';'
  | TK_VAR identifier [type] = expression ';'
  ;
*/
static node_t *variable_declaration(parser_t *p)
{
  const token_t *tok = NULL;
  node_t *expr = NULL;
  assert_next(p, TK_VAR);

  tok = expect(p, TK_IDENTIFIER);
  if (!tok) {
    return NULL;
  }
  printf("%s ", word_value_of(tok));

#if n
  tok = get_token(p);
  if (kind_of(tok) != TK_IDENT) {
    return NULL;
  }
  printf("%s ", word_value_of(tok));
#endif

  if (is_next(p, TK_INT)) {
  }

  if (is_next(p, '=')) {
    expr = expression(p);
    printf("%ld", expr->ivalue);
  }

  tok = expect(p, ';');
  if (!tok) {
    return NULL;
  }
  printf(";\n");
  /*
  printf("%s ", word_value_of(tok));
  if (expect(p, ';')) {
    printf("\n");
  }
  */

  return ast_var_decl(expr);
}
#endif

/*
program
  : statement_list
  ;
*/
static node_t *program(parser_t *p)
{
  return statement_list(p);
}

int parse_file(struct parser *p, const char *filename)
{
  const node_t *node = NULL;

  lex_input_file(&p->lex, filename);
  node = program(p);

  printf("PARSING DONE\n\n");
  ast_print_tree(node);

  return 0;
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
