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
	*ソフトウェア演習1*
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

= 考察
