#include <stdio.h>
#include <stdlib.h>

/*
main(int argc char **argv) コマンドラインで引数を渡すときに使う
argc : 引数の数（実行プログラムも入るので必ず1以上）
argv : ファイル名はargv[0], argv[1], argv[2]に指定する引数が入る、全部char型？っぽいのでその都度変換が必要なのかな
atoi : ascii to integer, 文字列（ASCII）を整数（Integer）に変換するための関数

$ cc -o 9cc 9cc.c : 最初の9ccが実行ファイルで、コンパイルするファイル名がその次に来る、これはintel記法に似てる
$ ./9cc 123 > tmp.s : 実行ファイルに値を渡す、今回は123、それをtmp.sに書き込む
*/

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }

    char *p = argv[1]; /* 1つ目に渡される引数の文字列の先頭のポインタ？ */

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");
    printf("  mov rax, %ld\n", strtol(p, &p, 10));

    while (*p) {
        if (*p == '+') {
            p++;
            printf("  add rax, %ld\n", strtol(p, &p, 10));
            continue;
        }

        if (*p == '-') {
            p++;
            printf("  sub rax, %ld\n", strtol(p, &p, 10));
            continue;
        }

        fprintf(stderr, "予期しない文字です: '%c'\n", *p);
        return 1;
    }

    printf("  ret\n");
    return 0;
}