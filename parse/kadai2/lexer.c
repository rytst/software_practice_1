/*
 * lexer.c
 *
 *   正規表現の字句解析ルーチン
 *
 */

#include <stdio.h>
#include "regmatch.h"

token curr_token;  /* 1番最近に読んだトークン */
char token_val;    /* トークンの意味値 */

/* lexer: 字句解析して結果を表示 */
int lexer()
{
  do {
    get_token();
    print_token(curr_token, token_val);
  } while (curr_token != EOREG);

  return 0;
}

/* get_token: 字句解析ルーチン */
void get_token() 
{
  int c;

  c = *reg_string;

  switch (c) {
  case '*':
    curr_token = AST;
    break;
  case '.':
    curr_token = CONC;
    break;
  case '(': 
    curr_token = LPAR;
    break;
  case ')': 
    curr_token = RPAR;
    break;
  case '|':
    curr_token = VERT;
    break;
  case '\0':
    curr_token = EOREG;
    break;
  case '\\':
    reg_string++;
    c = *reg_string;
    switch(c) {
    case '0': 
      curr_token = EMPTY;
      break;
    case 'e':
      curr_token = EPSILON;
      break;
    default:
      curr_token = LETTER;
      token_val = c;
      break;
    }
    break;
  default:
    curr_token = LETTER;
    token_val = c;
    break;
  }
  reg_string++;

  if (debug)
    print_token(curr_token,token_val);
}

/* print_token: curr_token を表示する */
void print_token(token tok, char c)
{
  switch (tok) {
  case EMPTY: 
    printf("EMPTY\n");
    break;
  case EPSILON: 
    printf("EPSILON\n");
    break;
  case AST: 
    printf("AST\n");
    break;
  case CONC: 
    printf("CONC\n");
    break;
  case LPAR: 
    printf("LPAR\n");
    break;
  case RPAR: 
    printf("RPAR\n");
    break;
  case VERT: 
    printf("VERT\n");
    break;
  case LETTER: 
    printf("LETTER(%c)\n", c);
    break;
  case EOREG: 
    printf("EOREG\n");
    break;
  default:
    fatal_error("Invalid token");
    break;
  }
}
