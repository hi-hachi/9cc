#!/bin/bash
assert() {
    expected="$1" # 1番目の引数　つまり assertの次にある1つ目の0
    input="$2"    # 2番目の引数

    ./9cc "$input" > tmp.s # 9ccコンパイラ実行、tmp.sを作る
    cc -o tmp tmp.s        # tmp.sからtmp実行ファイルを作る
    ./tmp
    actual="$?"            # $?は直前に実行したコマンドが成功したか失敗したかを値が入る　おそらく0~255?

    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

assert 0 0
assert 42 42
assert 21 "5+20-4"
assert 41 " 12 + 34 - 5 "

echo OK

# chmod a+x test.sh ... これをしないとコンピュータは実行できるファイルだと認識してくれないらしい