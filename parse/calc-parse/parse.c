/*
 * parse.c
 *  
 *       (足し算・かけ算のみの)整数算術式の構文解析ルーチン
 *  
 *       構文規則(２項演算子が右結合性を持つ場合)
 *       expr ::= term | term PLUS expr
 *       term ::= factor | factor TIMES term
 *       factor ::= primary | PLUS primary | MINUS primary
 *       primary ::= NUMBER | LPAR expr RPAR
 *
 */

#include <stdio.h>  /* printf */
#include <stdlib.h> /* malloc */
#include "calc.h"

int parse();               /* 構文解析して正規表現を評価 */
void parse_error();        /* 構文エラーで終了 */
ptree *eval_expr(void);    /* expr を評価 */
ptree *eval_term(void);    /* term を評価 */
ptree *eval_factor(void);  /* factor を評価 */
ptree *eval_primary(void); /* primary を評価 */

ptree *make_ptree(token tok, int val, ptree *left, ptree *right); /* 構文木を作成 */
void print_ptree(ptree *root); /* 構文木を表示 */
static void print_ptree_n(int n, ptree *root); /* print_ptree に使う補助関数 */

static int nest = 0; /* 再帰呼び出しのネストレベル */

/* parse: 構文解析して算術式を評価 */
int parse()
{
  ptree *root;

  get_token();
  root = eval_expr();
  if (curr_token != EOEXPR) {
    parse_error();
  } else {
    print_ptree(root);
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
  int i;

  if (debug) {
    for(i=0;i<nest;i++)
      printf("   ");
    printf("eval_expr(%d)\n",nest);
  }
  nest++;

  root = eval_term();

  if (curr_token == PLUS) {
    get_token();
    root = make_ptree(PLUS,0,root,eval_expr());
  }

  nest--;
  return root;
}

/* eval_term: term を評価 */
ptree *eval_term()
{
  ptree *root;
  int i;

  if (debug) {
    for(i=0;i<nest;i++)
      printf("   ");
    printf("eval_term(%d)\n",nest);
  }
  nest++;

  root = eval_factor();

  if (curr_token == TIMES) {
    get_token();
    root = make_ptree(TIMES,0,root,eval_term());
  }

  nest--;
  return root;
}

/* eval_factor: factor を評価 */
ptree *eval_factor()
{
  ptree *root;
  token token_sav;
  int i;

  if (debug) {
    for(i=0;i<nest;i++)
      printf("   ");
    printf("eval_factor(%d)\n",nest);
  }
  nest++;

  if (curr_token == PLUS || curr_token == MINUS) {
    token_sav = curr_token;
    get_token();
    root = make_ptree(token_sav,0,NULL,eval_primary());
  }
  else {
    root = eval_primary();
  }

  nest--;
  return root;
}

/* eval_primary: primary を評価 */
ptree *eval_primary()
{
  ptree *root;
  int i;

  if (debug) {
    for(i=0;i<nest;i++)
      printf("   ");
    printf("eval_primary(%d)\n",nest);
  }
  nest++;

  if (curr_token == NUMBER) {
    root = make_ptree(curr_token,token_val,NULL,NULL);
    get_token();  
  } else if (curr_token == LPAR) {
    get_token();
    root = eval_expr();
    if (curr_token != RPAR) {
      parse_error();
    } 
    get_token();  
  } else {
    parse_error();
  } 

  nest--;
  return root;
}


/* make_ptree: 構文木を作成 */
ptree *make_ptree(token tok, int val, ptree *left, ptree *right)
{
  ptree *root;

  if ((root = (ptree *)malloc(sizeof(ptree))) == NULL) {
    fatal_error("malloc error");
  }

  root->tok = tok;
  root->val = val;
  root->left = left;
  root->right = right;

  if (debug)
    print_token(root->tok, root->val);

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
