%{

/* 四則演算用の構文解析 */

#include <stdio.h>
#include <math.h>
#include <string.h>

#define MAX_N 128


int count;
/* push arguments of function */
double buffer[128];

/* 関数のプロトタイプ宣言 */
int yylex(void);
void yyerror(char *word);
void printAnswer(double val);
double execFunction(char *func, double *buffer, int count);

int debug = 1; /* 1にするとデバッグモード */


/* 関数名と関数へのポインタの関連付けのための構造体 */
typedef struct {
  char *symbol;            /* 関数名 */
  double (*func)(double);  /* １引数の関数へのポインタ */
} relation;

typedef struct {
  char *symbol;            /* 関数名 */
  double (*func)(double, double);  /* 2引数の関数へのポインタ */
} relation_for_2arg;

typedef struct {
  char *symbol;            /* 関数名 */
  double (*func)(double *, int);  /* n引数の関数へのポインタ(n >= 3) */
} relation_for_mult;


/* 半径 radius の円の面積を求める関数 */
double areaCircle(double radius) {
  return M_PI * radius * radius;
}


double max(double *buffer, int count) {
  double max_val = buffer[0];
  for (int i=1; i < count; i++) {
    if (max_val < buffer[i]) {
      max_val = buffer[i];
    }
  }
  return max_val;
}


double min(double *buffer, int count) {
  double min_val = buffer[0];
  for (int i=1; i < count; i++) {
    if (min_val > buffer[i]) {
      min_val = buffer[i];
    }
  }
  return min_val;
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
  { "areaCircle", &areaCircle }, /* ユーザ定義関数もOK */
  /* 同じ要領で関数を登録できる */
};

static relation_for_2arg table_for_2arg[] = {
  { "atan2", &atan2 },
  { "pow", &pow },
};


static relation_for_mult table_for_mult[] = {
  { "min", &min },
  { "max", &max },
};


%}

%union {
  double number;
  char *symbol;
}

%token <number> NUMBER
%token <symbol> FUNCTION
%token COMMA
%token PLUS MINUS TIMES DIV
%token LPAR RPAR
%token EOEXPR

%left EOEXPR
%right PLUS MINUS
%right TIMES DIV
%nonassoc SIGN

%type<number> expr
%type<number> vector

%%

// start:  start EOEXPR {
//   printf( "EOEXPR found\n" );
// } start {
//   printf( "end of rule\n" );
// }
start: start EOEXPR start
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
        | FUNCTION LPAR vector RPAR { $$ = execFunction($1, buffer, count); } /* 引数１個の関数の処理 */
        ;

vector:
        vector COMMA expr { count++ ; if (debug) printf("%d\n", count); buffer[count-1] = $3; }
        | expr { count = 1; if (debug) printf("%d\n", count); buffer[count-1] = $1; }

%%

void printAnswer(double val) {
  static int no = 0;
  printf(" [%d] %lf\n", ++no, val);
}

double execFunction(char *func, double *buffer, int count) {
  int i;
  if (count == 1) {
    for (i = 0; i < sizeof(table) / sizeof(relation); i++) {
      if ( strcmp( func, table[i].symbol ) == 0 ) { // tableのsymbolと一致したら
        if (debug) printf("call %s(%lf)\n", table[i].symbol, buffer[0]);
        return (*table[i].func)(buffer[0]); // 関数呼び出し
      }
    }
  } else if (count == 2) {
    for (i = 0; i < sizeof(table_for_2arg) / sizeof(relation_for_2arg); i++) {
      if ( strcmp( func, table_for_2arg[i].symbol ) == 0 ) { // tableのsymbolと一致したら
        if (debug) printf("call %s(%lf, %lf)\n", table_for_2arg[i].symbol, buffer[0], buffer[1]);
        return (*table_for_2arg[i].func)(buffer[0], buffer[1]);
      }
    } 
  } else {
    for (i = 0; i < sizeof(table_for_mult) / sizeof(relation_for_mult); i++) {
      if ( strcmp( func, table_for_mult[i].symbol ) == 0 ) { // tableのsymbolと一致したら
        if (debug) printf("call %s(buffer)\n", table_for_mult[i].symbol);
        return (*table_for_mult[i].func)(buffer, count);
      }
    }
  }
  fprintf( stderr, "unknown function (function name = %s)\n", func);
  return buffer[0];
}

void yyerror(char *msg)
{
  fprintf( stderr, "parse error at %s:%d (msg = %s)\n", __FILE__, __LINE__, msg );
}
