/*
 *  nfa.c
 *
 *  正規表現から非決定的有限オートマトンを生成し，表示する
 *
 */

#include <stdio.h>
#include "regmatch.h"

int epsilon = -1;             /* 空遷移を表わすラベル */
elist *nvlist[MAX_NVNUM];     /* NFAの頂点リスト */
int nvnum = 0;                /* vlist 中の有効なエントリ数 */
int initial_nv;               /* 初期頂点 */
int final_nv;                 /* 終了頂点 */

int make_nfa(void);           /* NFA を生成して表示する */
void gen_nfa(ptree *tp);      /* 構文木から NFA を生成 */
void print_nfa(void);         /* NFA(頂点リスト，初期頂点，終了頂点)を表示 */


/* NFA を表す構造体 */
typedef struct nfa {
  int start;
  int end;
} nfa;

/* NFA を生成する関数群 */
static nfa ptree_to_nfa(ptree *tp);
static nfa gen_empty_nfa(void);
static nfa gen_epsilon_nfa(void);
static nfa gen_literal_nfa(char val);
static nfa gen_conc_nfa(nfa n1, nfa n2);
static nfa gen_vert_nfa(nfa n1, nfa n2);
static nfa gen_ast_nfa(nfa n1);

static int gen_nvertex(void);   /* 新しい頂点を生成 */
static void add_nedge(int nv1, int nv2, int label); /* 辺を追加 */

/* NFAを作成し，表示する */
int make_nfa()
{
  ptree *root;

  get_token();
  root = eval_expr();

  if (curr_token == EOREG) {
    gen_nfa(root);
    print_nfa();
  } else {
    parse_error();
  }

  free_ptree(root);
  free_nfa();

  return 0;
}

/* 構文木から，NFAを作成：公開用 */
void gen_nfa(ptree *tp)
{
  nfa n;

  n = ptree_to_nfa(tp);
  initial_nv = n.start;
  final_nv = n.end;
}

/* NFA(頂点リスト，初期頂点，終了頂点)を表示 */
void print_nfa()
{
  int i;
  elist *elp;

  for (i = 0; i < nvnum; i++) {
    for (elp = nvlist[i]; elp != NULL; elp = elp->next) {
      if (elp->label == -1)
	 printf("\\e");
      else
	printf(" %c", elp->label);
      printf(": %3d => %3d\n", i, elp->node);
    }
  }
  printf("Initial state: %3d\n", initial_nv);
  printf("Final state: %3d\n", final_nv);
}

/*********************
 NFA を生成する関数群
**********************/

/* 構文木から，NFAを作成：再帰用*/
nfa ptree_to_nfa(ptree *tp)
{
  nfa n, n1, n2;

  switch (tp->tok) {
  case EMPTY:
    n = gen_empty_nfa();
    break;
  case EPSILON:
    n = gen_epsilon_nfa();
    break;
  case LETTER:
    n = gen_literal_nfa(tp->val);
    break;
  case CONC:
    n1 = ptree_to_nfa(tp->left);
    n2 = ptree_to_nfa(tp->right);
    n = gen_conc_nfa(n1, n2);
    break;
  case VERT:
    n1 = ptree_to_nfa(tp->left);
    n2 = ptree_to_nfa(tp->right);
    n = gen_vert_nfa(n1, n2);
    break;
  case AST:
    n1 = ptree_to_nfa(tp->left);
    n = gen_ast_nfa(n1);
    break;
  default:
    fatal_error("unexpected error at ptree_to_nfa");
    break;
  }

  return n;
}


nfa gen_empty_nfa()
{
  nfa n;

  n.start = gen_nvertex();
  n.end = gen_nvertex();

  return n;
}

nfa gen_epsilon_nfa(void)
{
  nfa n;

  n.start = gen_nvertex();
  n.end = gen_nvertex();
  add_nedge(n.start, n.end, epsilon);

  return n;
}

nfa gen_literal_nfa(char val)
{
  nfa n;

  n.start = gen_nvertex();
  n.end = gen_nvertex();
  add_nedge(n.start, n.end, val);

  return n;
}

nfa gen_conc_nfa(nfa n1, nfa n2)
{
  nfa n;

  n.start = n1.start;
  n.end = n2.end;
  add_nedge(n1.end, n2.start, epsilon);

  return n;
}

nfa gen_vert_nfa(nfa n1, nfa n2)
{
  nfa n;

  n.start = gen_nvertex();
  n.end = gen_nvertex();

  add_nedge(n.start, n1.start, epsilon);
  add_nedge(n.start, n2.start, epsilon);
  add_nedge(n1.end, n.end, epsilon);
  add_nedge(n2.end, n.end, epsilon);

  return n;
}

nfa gen_ast_nfa(nfa n1)
{
  nfa n;

  n.start = n1.start;
  n.end = gen_nvertex();

  add_nedge(n1.start, n.end, epsilon);
  add_nedge(n1.end, n1.start, epsilon);

  return n;
}

/* NFAの頂点を生成 */
int gen_nvertex()
{
  if (++nvnum >= MAX_NVNUM)
    fatal_error("too much NFA states");

  return nvnum - 1;
}

/* NFAの辺を追加 */
void add_nedge(int nv1, int nv2, int label)
{
  elist *elp, *elp2;

  if ((elp = (elist *)malloc(sizeof(elist))) == NULL)
    fatal_error("malloc error");

  if ((elp2 = nvlist[nv1]) ==  NULL)
    nvlist[nv1] = elp;
  else {
    for (; elp2->next != NULL; elp2 = elp2->next)
      ;
    elp2->next = elp;
  }

  elp->node = nv2;
  elp->label = label;
  elp->next = NULL;
}

void free_nfa()
{
  int i;
  elist  *elp, *next;

  for (i = 0; i < nvnum; i++) {
    for (elp = nvlist[i]; elp != NULL; elp = next) {
      next = elp->next;
      free(elp);
    }
  }
}
