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



= プログラムの流れの説明



= 考察


