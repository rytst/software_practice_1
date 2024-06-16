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
	第4回課題*
])

#align(center, text[
	J2200071 齊藤 隆斗
])


#show figure.where(
  kind: table
): set figure.caption(position: top)


#set heading(numbering: "1.")

= 課題5.4, 5.9, 5.10 のプログラム
*課題5.4*  関数 get_dvertex() 関数を与えよ.

*課題5.9* check_dvertex 関数中の、辺テーブルに基づいてDFA頂点/辺をdvlistに登録する部分の処理を追加して、
check_dvertex 関数を完成させよ.


*課題5.10* NFAからDFAを生成するプログラムを完成させ、オプションで指定することによって、
生成されたDFAをも表示できるようにメイン関数を変更せよ.



= 実行結果

実行例1: スライドの例について正常に動作するかどうかを確認
```
$ ./kadai4 -d4 'a|b*|c'
DFA
Number of DFA states: 4
Initial state: 0
(c) 0 => 3
(b) 0 => 2
(a) 0 => 1
(b) 2 => 2
Final states: 0 1 2 3
```


実行例2: スライドの例について正常に動作するかどうかを確認
```
$ ./kadai4 -d4 'a.b*|(d.e)*'
DFA
Number of DFA states: 5
Initial state: 0
(d) 0 => 2
(a) 0 => 1
(b) 1 => 3
(e) 2 => 4
(b) 3 => 3
(d) 4 => 2
Final states: 0 1 3 4
```


実行例3: その他の例についても正常に動作するかどうかを確認
```
$ ./kadai4 -d4 'a.\e.b'
DFA
Number of DFA states: 3
Initial state: 0
(a) 0 => 1
(b) 1 => 2
Final states: 2
```

実行例4: その他の例についても正常に動作するかどうかを確認
```
$ ./kadai4 -d4 '(a|\0)*'
DFA
Number of DFA states: 2
Initial state: 0
(a) 0 => 1
(a) 1 => 1
Final states: 0 1
```


実行例5: その他の例についても正常に動作するかどうかを確認
```
$ ./kadai4 -d4 '(a.b|c)*'
DFA
Number of DFA states: 4
Initial state: 0
(c) 0 => 2
(a) 0 => 1
(b) 1 => 3
(c) 2 => 2
(a) 2 => 1
(c) 3 => 2
(a) 3 => 1
Final states: 0 2 3
```


= プログラムの流れの説明
今回の課題はNFAからDFAを生成するプログラムが主なテーマであるから、オプションとして d1, d2, d3 が入力された場合のプログラムの流れについては省略する.
ここでは d4 がオプションとして入力された場合におけるプログラムの流れについて説明する.
オプション d4 が選択された場合、プログラムは関数 make_dfa() を実行して停止する.
したがって、関数 make_dfa() について見ていく.
関数 make_dfa() では、まず関数 eval_expr() によって、構文木を生成する.
そして、この時に curr_token の値がEOREGであれば、正常に構文木を生成できたということであるから、次の処理に移る.
curr_token の値がEOREGでなければ、正常に構文木を生成できなかったということであるから parse_error() を
呼び出し、エラー処理を行う. 正常に構文木が生成された後は、生成した構文木を 関数 gen_nfa() によって
NFAへ変換する. そして、この変換したNFAを関数 gen_dfa() によってDFAへ変換する. このgen_dfa() については
考察のセクションで扱う. DFAへ変換した後はDFAを表示し、確保していたメモリ領域を開放して、処理を停止する.



= 考察
このセクションでは、関数 gen_dfa() についての説明を行う. \
NFAをDFAへ変換するためには、改めて頂点を決定する必要がある.
ここで、DFAの頂点は、NFAの頂点の集合であり、
あるDFAの頂点があった場合、その頂点を構成するようなNFAの頂点から同一の文字で遷移できるような
NFAの頂点集合がDFAの頂点となる. \
これを踏まえた上で、まずはじめに、gen_dfa() では、

```c
	dv = gen_dvertex(); /* dv == 0 */
	enhance_nvset(initial_nv, dvlist[dv].nvset);
```

によって、DFAの初期状態を生成する. ここで、```c gen_dvertex()``` によってDFAの頂点を生成し、
```c enhance_nvset(initial_nv, dvlist[dv].nvset)``` によって、NFAの初期状態と
そこから $epsilon$ 遷移で到達できる頂点の集合を、先程生成したDFAの頂点に与えていることに注意する. \
次に、先ほど生成した初期状態の頂点からDFAを構成していく.
構成の手順は次のようである.

+ チェック済みでないような頂点があるかどうかを調べる.
	+ ない場合、処理を停止する.
	+ あった場合、そのDFAの頂点から出るような辺と行き先の頂点を生成し、チェック済みでなかった頂点をチェックする.

上の処理を行うのが下のコード部分である.

```c
	while ( (dv = get_dvertex()) != NOT_FOUND ) {
		check_dvertex(dv);
		dvlist[dv].check = 1;
	}
}
```

ここで、どのようにDFAの頂点から出るような辺と行き先の頂点を生成するかについて考察する.
ここで、辺テーブルというものを作成する.
ラベルごとに、ある頂点から出発して遷移するような頂点集合によってこの表を埋めていく.
例として、以下の図のようなNFAを考える.

#figure(
  image("NFA.png", width: 80%),
  caption: [
      NFA
  ],
)

このとき、この図において頂点集合 ${0, 1}$ に関する辺テーブルは以下のように作成していく.


+ 0から3へ遷移するラベルaの辺があり、まだ辺テーブルにラベルaの行はないので新たにラベルaで遷移先頂点集合が{3}の行を追加する.
+ さらに0から2へ遷移するラベルaの辺があり、すでに辺テーブルにラベルaの行が存在するので遷移先頂点集合に2を追加する.
+ 1から3へ遷移するラベルbの辺があり、まだ辺テーブルにラベルbの行はないので新たにラベルbで遷移先頂点集合が{3}の行を追加する.
+ さらに1から2へ遷移するラベルbの辺があり、すでに辺テーブルにラベルbの行が存在するので遷移先頂点集合に2を追加する.

このようにして、以下の辺テーブルを作成することができる.

#figure(
    table(
	    columns: 3,
		stroke: (x: none),
		row-gutter: (2.2pt, auto),
		table.header[出発頂点][辺][遷移先頂点集合],
		[0], [a], [{2, 3}],
		[1], [b], [{2, 3}],
	),
	caption: [etable],
)

この表から、頂点集合 ${0, 1}$ から頂点集合 ${2, 3}$ へラベルaとラベルbによって遷移するような辺が2つあることがわかる.


