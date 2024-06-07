/*
 * main.c
 *  
 *   オプション
 *  
 *  -d1 : トークン列(字句解析)
 *  -d2 : 構文木(構文解析)
 *  -d3 : NFA (非決定性有限オートマトン)
 *  -d4 : DFA (決定性有限オートマトン)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "regmatch.h"

void print_usage();
char *program_name;
int debug = 0;    /* デバッグ情報を表示する(1)/しない(0) */
char *reg_string; /* 正規表現入力 */


int main( int argc, char *argv[] )
{
  program_name = argv[0];
  
  if ( argc <= 2 ) {
    print_usage();
    fatal_error("argument required");
  }
  
  reg_string = argv[2];

  if ( argv[1][0] == '-' && argv[1][1] == 'd' ) {
    int option = atoi( &argv[1][2] );
    switch(option){
    case 1:
      printf("トークン列(字句解析結果)\n");
      lexer();
      break;
    case 2:
      printf("構文木(構文解析結果)\n");
      parse();
      break;
    case 3:
      printf("NFA(非決定性有限オートマトン)\n");
      make_nfa();
      break;
    case 4:
      /*
       *  課題5.10 DFA(決定性有限オートマトン)を作るための処理
       */
      printf("DFA(決定性有限オートマトン)\n");
      make_dfa();
      break;
    default:
      print_usage();
      fatal_error("invalid argument");
    }
  }
  else {
    print_usage();
    fatal_error("invalid argument");
  }
  
  return 0;
}

void print_usage() {
  printf("Usage : %s -d{1|2|3|4} 'regular expression'\n", program_name );
  printf("      -d1 : トークン列(字句解析結果)\n");
  printf("      -d2 : 構文木(構文解析結果)\n");
  printf("      -d3 : NFA(非決定性有限オートマトン)\n");
  printf("      -d4 : DFA(決定性有限オートマトン)\n");
}

/* fatal_error: エラーメッセージを表示して終了 */
void fatal_error(char *s)
{
  fprintf(stderr, "%s\n", s);
  exit(1);
}

