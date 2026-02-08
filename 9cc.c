#include <stdio.h>
#include <stdlib.h>

/* memo
main(int argc char **argv) コマンドラインで引数を渡すときに使う
argc : 引数の数（実行プログラムも入るので必ず1以上）
argv : ファイル名はargv[0], argv[1], argv[2]に指定する引数が入る、全部char型？っぽいのでその都度変換が必要なのかな
atoi : ascii to integer, 文字列（ASCII）を整数（Integer）に変換するための関数

$ cc -o 9cc 9cc.c : 最初の9ccが実行ファイルで、コンパイルするファイル名がその次に来る、これはintel記法に似てる
$ ./9cc 123 > tmp.s : 実行ファイルに値を渡す、今回は123、それをtmp.sに書き込む
*/

// トークンの種類 (enumは定数が0, 1, 2... と割り当てられる)
typedef enum {
    TK_RESERVED, // 記号 
    TK_NUM,      // 整数トークン
    TK_EOF       // 入力の終わり
} TokenKind;

typedef struct Token Token;

// トークン型
struct Token {
    TokenKind kind; // トークンの型
    Token *next;    // 次の入力トークン
    int val;        // kindがTK_NUMの場合、その数値
    char *str;      // トークンの文字列
};

// 現在着目してるトークン
Token *token;

// エラーを報告するための関数
// printfと同じ引数をとる
void error (char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "\n");
    exit(1);
}

/*
次のトークンが期待している記号のときには、トークンを1つ読み進めて真を返す。
それ以外の場合には偽を返す。
*/
bool consume(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op) {
        return false;
    }
    token = token->next;
    return true;
}

/*
次のトークンが期待している記号のときには、トークンを1つ読み進める。
それ以外の場合にはエラーを返す。
*/
bool expect(char op) {
    if (token->kind != TK_RESERVED || token->str[0] != op) {
        error("'%c'ではありません", op);
    }
    token = token->next;
}

/*
次のトークンが数値の場合、トークンを1つ読み進めてその数値を返す。
それ以外の場合にはエラーを返す。
*/
int expect_number() {
    if (token->kind != TK_NUM) {
        error("数ではありません");
    }
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

// 新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str) {
    Token *tok = calloc(1, sizeof(Token)); // メモリ確保+ゼロ初期化をしてくれる
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
}

// 入力文字列pをトークナイズしてそれを返す
Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        // 空白文字をスキップ
        if (isspace(*p)) {
        p++;
        continue;
        }

        if (*p == '+' || *p == '-') {
        /* 文字が+or-だったら、記号のトークンの情報を付与 */
        cur = new_token(TK_RESERVED, cur, p++);
        continue;
        }

        if (isdigit(*p)) { //　渡された文字が0~9かどうかを判定
        cur = new_token(TK_NUM, cur, p);
        cur->val = strtol(p, &p, 10);
        continue;
        }

        error("トークナイズできません");
    }

    new_token(TK_EOF, cur, p);
    return head.next;
}

int main(int argc, char **argv) {
  if (argc != 2) { // argcは引数の数が入る、今回はプログラム名とコンパイルする文字列の2つ？
    error("引数の個数が正しくありません");
    return 1;
  }

  // トークナイズする
  token = tokenize(argv[1]);

  // アセンブリの前半部分を出力
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  // 式の最初は数でなければならないので、それをチェックして
  // 最初のmov命令を出力
  printf("  mov rax, %d\n", expect_number());

  // +じゃないなら絶対-がくるということを想定している
  // アセンブリを出力
  while (!at_eof()) {
    if (consume('+')) {
      printf("  add rax, %d\n", expect_number());
      continue;
    }

    expect('-');
    printf("  sub rax, %d\n", expect_number());
  }

  printf("  ret\n");
  return 0;
}
