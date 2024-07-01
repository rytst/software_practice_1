%{

/* 四則演算用の構文解析 */

#include <stdio.h>
#include <math.h>
#include <string.h>

/* 関数のプロトタイプ宣言 */
int yylex(void);
void yyerror(char *word);
void printAnswer(double val);
double execFunction(char *func, double val);

int debug = 0; /* 1にするとデバッグモード */


/* 関数名と関数へのポインタの関連付けのための構造体 */
typedef struct {
  char *symbol;            /* 関数名 */
  double (*func)(double);  /* １引数の関数へのポインタ */
} relation;

/* 半径 radius の円の面積を求める関数 */
double areaCircle(double radius) {
  return M_PI * radius * radius;
}

/* 実際の関数名と関数ポインタの対応付けを定義 */
static relation table[] = {
  { "log",  &log  },
  { "exp",  &exp  },
  { "sin",  &sin  },
  { "cos",  &cos  },
  { "tan",  &tan  },
  { "asin", &asin },
  { "acos", &acos },
  { "atan", &atan },
  { "areaCircle", &areaCircle} /* ユーザ定義関数もOK */
  /* 同じ要領で関数を登録できる */
};

%}

%union {
  double number;
  char *symbol;
}

%token <number> NUMBER
%token <symbol> FUNCTION
%token PLUS MINUS TIMES DIV
%token LPAR RPAR
%token EOEXPR

%right EOEXPR
%right PLUS MINUS
%right TIMES DIV
%nonassoc SIGN

%type<number> expr

%%

start:  start EOEXPR {
  printf( "EOEXPR found\n" );
} start {
  printf( "end of rule\n" );
}
        | expr { printAnswer($1); }
        |
        ;

expr:
        expr PLUS expr     { $$ = $1 + $3; if (debug) printf("%lf = %lf + %lf\n", $$, $1, $3); }
        | expr MINUS expr  { $$ = $1 - $3; if (debug) printf("%lf = %lf - %lf\n", $$, $1, $3); }
        | expr TIMES expr  { $$ = $1 * $3; if (debug) printf("%lf = %lf * %lf\n", $$, $1, $3); }
        | expr DIV expr    { $$ = $1 / $3; if (debug) printf("%lf = %lf / %lf\n", $$, $1, $3); }
        | NUMBER           { $$ = $1; }
        | PLUS expr  %prec SIGN { $$ = $2; if (debug) printf("%lf = + %lf\n", $$, $2); }
        | MINUS expr %prec SIGN { $$ = -$2; if (debug) printf("%lf = - %lf\n", $$, $2); }
        | LPAR expr RPAR   { $$ = $2; }
        | FUNCTION LPAR expr RPAR { $$ = execFunction($1, $3); } /* 引数１個の関数の処理 */
        ;

%%

void printAnswer(double val) {
  static int no = 0;
  printf(" [%d] %lf\n", ++no, val);
}

double execFunction(char *func, double val) {
  int i;
  for (i = 0; i < sizeof(table) / sizeof(relation); i++) {
    if ( strcmp( func, table[i].symbol ) == 0 ) { // tableのsymbolと一致したら
      if (debug) printf("call %s(%lf)\n", table[i].symbol, val);
      return (*table[i].func)(val); // 関数呼び出し
    }
  }
  fprintf( stderr, "unknown function (function name = %s)\n", func);
  return val;
}

void yyerror(char *msg)
{
  fprintf( stderr, "parse error at %s:%d (msg = %s)\n", __FILE__, __LINE__, msg );
}
