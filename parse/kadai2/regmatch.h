/*
 * regmatch.h
 *
 *   正規表現のヘッダファイル
 *
 */

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

typedef struct ptree {    /* 構文木 */
  token tok;
  char val;
  struct ptree *left;
  struct ptree *right;
} ptree;

/* from main.c */
extern int debug;         /* デバッグ情報を表示する(1)/しない(0) */
extern char *reg_string;  /* 正規表現文字列 */
extern void fatal_error(char *s); /* エラーメッセージを表示して終了 */

/* from lexer.c */
extern token curr_token;  /* 1番最近に読んだトークン */
extern char token_val;     /* トークンの意味値 */
extern int lexer(void);          /* 字句解析結果を表示 */
extern void get_token(void);     /* 字句解析ルーチン */
extern void print_token(token tok, char c); /* token を表示する */

/* from parse.c */
extern int parse(void);           /* 構文解析して正規表現を評価 */
extern void parse_error(void);    /* 構文エラーで終了 */
extern void free_ptree(ptree *root);
