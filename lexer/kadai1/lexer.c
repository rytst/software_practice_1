/*
 * lexer.c
 *  
 *   算術式の字句解析ルーチン
 *  
 */
#include <stdio.h>  /* fprintf */
#include <ctype.h>  /* isdigit */
#include <stdlib.h> /* atoi */
#include "regmatch.h"

token curr_token;  /* 1番最近に読んだトークン */
// int token_val;     /* トークンの意味値 */
char token_val;


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
void get_token(void)
{
/*int i; */
  char c /*, s[BUFSIZ]*/ = *reg_string; /* BUFSIZ は OS で標準に利用されるバッファーサイズ */

  /* 算術式なので、間の空白は飛ばす */
/*while ((s[0] = (c = *reg_string)) == ' ' || c == '\t' || c == '\n')
    reg_string++;*/

	switch (c) {
		case '\\':
			c = *++reg_string;
			switch (c) {
				case '0':
					curr_token = EMPTY;
					break;

				case 'e':
					curr_token = EPSILON;
					break;

				case '\0':
					curr_token = EOREG;
					break;

				default:
					token_val = c;
					curr_token = LETTER;
					break;
			}
			break;

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

		default:
			token_val = c;
			curr_token = LETTER;
			break;

	}

	reg_string++;

/*if (!isdigit(c)) { 
    switch (c) {
    case '+':
      curr_token = PLUS; 
      break;
    case '-': 
      curr_token = MINUS; 
      break;
    case '*': 
      curr_token = TIMES;
      break;
    case '/': 
      curr_token = DIV; 
      break;
    case '(': 
      curr_token = LPAR; 
      break;
    case ')': 
      curr_token = RPAR; 
      break;
    case '\0': 
      curr_token = EOEXPR;
      break;
    default: 
      fatal_error("Invalid charater\n"); 
      break;
    }
    reg_string++;

  } else {
    i = 0;
    while (isdigit(s[++i] = *++reg_string))
      ;
    s[i] = '\0';
    token_val = atoi(s);
    curr_token = NUMBER;
  }*/

  if (debug) 
    print_token(curr_token,token_val);
}

/* print_token: token を表示する */
void print_token(token tok, /*int val*/ char val)
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

		case EOREG:
			printf("EOREG\n");
			break;

		default:
			printf("LETTER(%c)\n", val);
			break;

	}
/*switch (tok) {
  case PLUS: 
    printf("PLUS\n");
    break;
  case MINUS: 
    printf("MINUS\n");
    break;
  case TIMES: 
    printf("TIMES\n");
    break;
  case DIV: 
    printf("DIV\n");
    break;
  case LPAR: 
    printf("LPAR\n");
    break;
  case RPAR: 
    printf("RPAR\n");
    break;
  case EOEXPR: 
    printf("EOEXPR\n");
    break;
  case NUMBER: 
    printf("NUMBER(%d)\n", val);
    break;
  default:
    fatal_error("Invalid token");
    break;
  }*/
}
