/*
 * parse.c
 *  
 *   構文解析ルーチン
 *  
 * 構文規則
 *  
 *   expr ::= term | term VERT expr
 *   term ::= factor | factor CONC term
 *   factor ::= primary | primary AST
 *   primary ::= LETTER | LPAR expr RPAR
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "regmatch.h"

int parse(void);           /* 構文解析して正規表現を評価 */
void parse_error(void);    /* 構文エラーで終了 */
ptree *eval_expr(void);    /* expr を評価 */
ptree *eval_term(void);    /* term を評価 */
ptree *eval_factor(void);  /* factor を評価 */
ptree *eval_primary(void); /* primary を評価 */

ptree *make_ptree(token tok, char val, ptree *left, ptree *right); /* 構文木を作成 */
void print_ptree(ptree *root); /* 構文木を表示 */
static void print_ptree_n(int n, ptree *root); /* print_ptree に使う補助関数 */


/* parse: 構文解析して正規表現を評価 */
int parse()
{
  ptree *root;

  get_token();
  root = eval_expr();

  if (curr_token == EOREG) {
    print_ptree(root);
  } else {
    parse_error();
  }

  free_ptree(root);
  return 0;
}

/* parse_error: 構文エラーで終了 */
void parse_error()
{
  fatal_error("syntax error");
}

/* eval_expr: expr を評価 */
ptree *eval_expr()
{
  ptree *root;

  root = eval_term();

  if (curr_token == VERT) {
    get_token();
    root = make_ptree(VERT,0,root,eval_expr());
  }
  
  return root;
}

/* eval_term: term を評価 */
ptree *eval_term()
{
  ptree *root;

  root = eval_factor();

  if (curr_token == CONC) {
    get_token();
    root = make_ptree(CONC,0,root,eval_term());
  }

  return root;
}

/* eval_factor: factor を評価 */
ptree *eval_factor()
{
  ptree *root;

  root = eval_primary();

  if (curr_token == AST) {
    root = make_ptree(AST,0,root,NULL);
    get_token();
  }

  return root;
}

/* eval_primary: primary を評価 */
ptree *eval_primary()
{
  ptree *root;

  if (curr_token == LETTER || curr_token == EPSILON || curr_token == EMPTY) {
    root = make_ptree(curr_token,token_val,NULL,NULL);
    get_token();  
  }
  else if (curr_token == LPAR) {
    get_token();
    root = eval_expr();
    if (curr_token != RPAR) {
      parse_error();
    } 
    get_token();  
  } else {
    parse_error();
  }

  return root;
}

/* make_ptree: 構文木を作成 */
ptree *make_ptree(token tok, char val, ptree *left, ptree *right)
{
  ptree *root;

  if ((root = (ptree *)malloc(sizeof(ptree))) == NULL) {
    fatal_error("malloc error");
  }

  root->tok = tok;
  root->val = val;
  root->left = left;
  root->right = right;

  return root;
}

/* print_ptree: 構文木を表示 */
void print_ptree(ptree *root)
{
  print_ptree_n(0, root);
}

/* print_ptree_n: print_ptree に使う補助関数 */
void print_ptree_n(int n, ptree *root)
{
  int i;
  if (root == NULL)
    return;

  print_ptree_n(n+1,root->left);

  for(i=0;i<n;i++)
    printf("   ");
  print_token(root->tok,root->val);

  print_ptree_n(n+1,root->right);

}

void free_ptree(ptree *root)
{
  if (root->left)  free_ptree(root->left);
  if (root->right) free_ptree(root->right);
  free(root);
}
