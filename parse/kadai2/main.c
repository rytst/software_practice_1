/*
 * main.c
 *
 *   正規表現構文解析プログラムのメインルーチン
 *
 *
 */

#include <stdio.h>  /* fprintf */
#include <stdlib.h> /* exit */
#include <string.h>
#include "regmatch.h"

int debug = 0;     /* デバッグ情報を表示する(1)/しない(0) */
char *reg_string;  /* 正規表現文字列 */
char *option;

int main(int argc, char *argv[])
{
  /* debug = 1;*/

  if (argc < 2)
    fatal_error("no argument");


  if (debug) 
    printf("Regular expression: %s\n", reg_string);

  if (argc == 2) {
    reg_string = argv[1];
    parse();
  } else if (argc == 3) { // for assignment 3.8
    option = argv[1];
    reg_string = argv[2];

    if (strcmp(option, "-d0") == 0) {
      printf("%s\n", reg_string);
    } else if (strcmp(option, "-d1") == 0) {
      lexer();
    } else if (strcmp(option, "-d2") == 0) {
      parse();
    } else {
      fatal_error("invalid option");
	}
  } else {
    fatal_error("too many arguments");
  }

  return 0;
}

/* fatal_error: エラーメッセージを表示して終了 */
void fatal_error(char *s)
{
  fprintf(stderr, "%s\n", s);
  exit(1);
}
