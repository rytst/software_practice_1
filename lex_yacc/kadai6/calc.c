#include <stdio.h>

extern int yyparse();
extern FILE *yyin;


int main() {
  // yyin = stdin; /* 標準入力から読み込む．ファイルから読み込むなら yyin = fopen(filename, "r"); */
  yyin = fopen("test.txt", "r");

  yyparse();    /* 構文解析 */

  return 0;
}
