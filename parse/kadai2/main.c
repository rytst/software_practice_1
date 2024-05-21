/*
 * main.c
 *
 *   正規表現構文解析プログラムのメインルーチン
 *
 *
 */

#include <stdio.h>  /* fprintf */
#include <stdlib.h> /* exit */
#include "regmatch.h"

int debug = 0;     /* デバッグ情報を表示する(1)/しない(0) */
char *reg_string;  /* 正規表現文字列 */

int main(int argc, char *argv[])
{
  /* debug = 1;*/

  if (argc != 2)
    fatal_error("no argument");

  reg_string = argv[1];

  if (debug) 
    printf("Regular expression: %s\n", reg_string);

  parse(); 
  return 0;
}

/* fatal_error: エラーメッセージを表示して終了 */
void fatal_error(char *s)
{
  fprintf(stderr, "%s\n", s);
  exit(1);
}
