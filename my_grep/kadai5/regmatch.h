/*
 * regmatch.h 
 *    
 *   正規表現の構文木を表示するプログラム
 *      
 */ 

#include <stdio.h>  /* fprintf, getc, ungetc */
#include <ctype.h>  /* isdigit */
#include <stdlib.h> /* atof, div*/


/* from main.c */
extern int debug;         /* デバッグ情報を表示する(1)/しない(0) */

/***** 字句解析ルーチン ******/

/* トークン */
typedef enum token { 
  EMPTY,
  EPSILON,
  AST,
  CONC,
  LPAR,
  RPAR,
  VERT,
  LETTER,
  EOREG
} token;

/* from lexer.c */
extern token curr_token;  /* 1番最近に読んだトークン */
extern char token_val;    /* トークンの意味値 */
extern char *reg_string;  /* 正規表現入力 */

extern int lexer(void);           /* 字句解析結果を表示するだけの関数 */
extern void get_token(void);      /* 字句解析ルーチン */
extern void fatal_error(char *s); /* エラーメッセージを表示して終了 */
extern void print_token(token tok, char c); /* token を表示する */

/***** 構文解析ルーチン ******/

typedef struct ptree {
  token tok;
  char val;
  struct ptree *left;
  struct ptree *right;
} ptree;

/* from parse.c */
extern int parse(void);
extern void parse_error();        /* 構文エラーで終了 */
extern ptree *eval_expr(void);    /* expr を評価 */

/*****  非決定的オートマトン生成ルーチン *****/

typedef struct elist {
  int node;
  int label;
  struct elist *next;
} elist;

#define MAX_NVNUM 100  /* NFA の頂点数の最大 */

/* from nfa.c */
extern elist *nvlist[MAX_NVNUM]; /* NFAの頂点リスト */
extern int nvnum;                /* nvlist 中の有効なエントリ数 */
extern int make_nfa(void);       /* NFA を生成して表示する */
extern void gen_nfa(ptree *tp);  /* 構文木から NFA を生成 */
extern void print_nfa(void);     /* NFA(頂点リスト，初期頂点，終了頂点)を表示 */
extern int epsilon;

/*****  決定的オートマトンへの変換ルーチン *****/

typedef struct dvertex { /* DFA の頂点 */
  unsigned char *nvset; /* NFAの頂点集合 */
  elist *elp;  /* 辺リストポインタ */
  char check;  /* 遷移先はチェック済みか */
} dvertex;

#define MAX_DVNUM 100              /* DFA の頂点数の最大 */

/* from dfa.c*/
extern dvertex dvlist[MAX_DVNUM];  /* DFAの頂点リスト */
extern int dvnum;                  /* dvlist 中の有効なエントリ数 */
extern int initial_nv;             /* 初期頂点 */
extern int final_nv;               /* 終了頂点 */
extern int make_dfa(void);         /* 決定的有限オートマトンを生成して表示 */
extern void gen_dfa(void);         /* 決定的有限オートマトンを生成 */
extern void print_dfa();           /* 決定的有限オートマトンを表示 */

extern void free_ptree(ptree *root);
extern void free_nfa();
extern void free_dfa();
