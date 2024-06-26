#set page(
  paper: "us-letter",
  numbering: "1",
)

#set par(justify: true)
#set text(
	lang: "ja",
	font: ("Linux Libertine", "Harano Aji Mincho"),
	size: 11pt,
)

#align(center, text(17pt)[
	*ソフトウェア演習1 \
	第3回課題*
])

#align(center, text[
	J2200071 齊藤 隆斗
])

#set heading(numbering: "1.")

= 課題4.5, 課題4.7-4.9のプログラム
*課題4.5* \
頂点nv1, nv2がすであったとして、ラベルlabelをもつ、頂点nv1から頂点nv2への辺をグラフ追加する関数add_nedge作成せよ. \
*課題4.7* \
残りの関数を記述せよ. \
*課題4.8* \
ptree_to_nfa関数を記述せよ. \
*課題4.9* \
構文木からNFAを生成するプログラムを完成させ、オプションで指定することによって、空遷移を含むNFA(ラベル付き有向グラフ+初期頂点+受理頂点)
も表示できるようにmain関数を変更せよ. \


= 実行結果

実行例1: テキストの例について正常に動作するかどうかを確認
```
$ ./kadai3 -d3 'a|b*|c'
 a:   0 =>   1
\e:   1 =>  10
 b:   2 =>   3
\e:   2 =>   4
\e:   3 =>   2
\e:   4 =>   8
 c:   5 =>   6
\e:   6 =>   8
\e:   7 =>   2
\e:   7 =>   5
\e:   8 =>  10
\e:   9 =>   0
\e:   9 =>   7
Initial state:   9
Final state:  10
```


実行例2: テキストの例について正常に動作するかどうかを確認
```
$ ./kadai3 -d3 'a.b*|c'
 a:   0 =>   1
\e:   1 =>   2
 b:   2 =>   3
\e:   2 =>   4
\e:   3 =>   2
\e:   4 =>   8
 c:   5 =>   6
\e:   6 =>   8
\e:   7 =>   0
\e:   7 =>   5
Initial state:   7
Final state:   8
```


実行例3: EMPTYが含まれる場合に正常に動作するかどうかを確認
```
$ ./kadai3 -d3 '\0.a'
\e:   1 =>   2
 a:   2 =>   3
Initial state:   0
Final state:   3
```

実行例4: EPSILONが含まれる場合に正常に動作するかどうかを確認
```
$ ./kadai3 -d3 '\e*'
\e:   0 =>   1
\e:   0 =>   2
\e:   1 =>   0
Initial state:   0
Final state:   2
```

実行例5: 追加の例について正常に動作するかどうかを確認
```
$ ./kadai3 -d3 '(a|b)*.c'
 a:   0 =>   1
\e:   1 =>   5
 b:   2 =>   3
\e:   3 =>   5
\e:   4 =>   0
\e:   4 =>   2
\e:   4 =>   6
\e:   5 =>   4
\e:   6 =>   7
 c:   7 =>   8
Initial state:   4
Final state:   8
```


= プログラムの流れの説明
今回のプログラムでは構文木からNFAに変換するようなプログラムを作成したので、
d1, d2オプションが指定された場合のプログラムの流れは省略する.
ここでは、d3オプションが指定された場合、すなわち構文木をNFAへ変換し、
表示する関数make_nfa()についてプログラムの流れについて説明する. \
まず、第一回の課題で作成した関数get_token()で、１つ目のトークンを取得する.
その後、第2回課題で作成した関数eval_expr()によって、そのトークンから構文木を生成する.
この構文木が生成し終わった時点でcurr_tokenの値がEOREGでない場合は解析エラーであるから、
parse_error()関数によってエラー処理を行う.
正常に解析ができた場合は、関数gen_nfa()によってNFAを構成する.
この関数gen_nfa()の詳細な流れは考察で触れる.その後、構成したNFAを表示する.
そして、構文解析木とNFAで確保していたメモリを開放し、処理を停止する.


= 考察
今回の課題で考えたこと、工夫したことは2つある. \
1つ目は関数gen_nfa()のアイデアである.
この関数はptree_to_nfa()によって、得られた正規表現の構文解析木をNFAに変換し、
ptree_to_nfa()の関数の終了後に初期頂点と終了頂点をセットしてこの関数の処理を終了する.
ここで、関数ptree_to_nfa()が再帰的に呼び出されることに注意する.
例として正規表現$a|b$の場合について見ていく.
この正規表現を構文解析した場合、ルートに$|$、ルートの左の子に$a$、
ルートの右の子に$b$となるような木が生成される.
よって、最初はptree_to_nfa()にはルートのトークンであるVERTが引数として渡される.
これはcase文によって、VERTの処理が行われる.ここでは、まずptree_to_nfa()に左の子を引数に渡し処理を行う.
すなわち、LETTER(a)のNFAを生成する.この際にこのNFAの初期頂点と終了頂点の情報を含む構造体をn1に格納しておく.
右の子LETTER(b)についても同様の処理をおこない、
この時に部分的に生成したNFAの初期状態と終了状態の情報を含む構造体をn2に格納しておく.
n1, n2の部分的に生成したNFAの初期状態と終了状態の情報と部分的に生成した2つのNFAから
gen_vert_nfa()関数によって$a|b$のNFAを構成する. このようにNFAを再帰的に構成される流れについて考えた. \
2つ目は関数add_nedge()の実装である.add_nedge()はnv1, nv2, labelを
引数にとり、nv1に、辺の行き先であるノードnv2とラベル情報labelが付与された辺を追加する.
この辺を追加する際には、まずノードnv1に辺があるかどうかを調べる必要がある.
これは、すでに他の辺が追加されている場合とまだ辺が追加されていない場合とで処理を分ける必要があるからである.
まだ辺が追加されていない場合は、nv1に辺の行き先nv2と辺のラベルlabelの情報を付与した要素をリンクすれば良い.
すでに辺が追加されている場合は、nv1にすでにリンクされている要素を辿っていき、
次の要素へのポインタがNULLの要素に、nv1にnv2とlabelの情報を付与した要素をリンクする.
このような流れによってノードnv1からノードnv2につながるようなラベルlabelが付与された辺を
追加することができると考えた.


