/*
 * regmath.h
 *
 *   正規表現の字句解析ルーチンヘッダファイル
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
