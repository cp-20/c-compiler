# 1cc (C言語で作るCコンパイラ)

[低レイヤを知りたい人のためのCコンパイラ作成入門](https://www.sigbus.info/compilerbook) に沿って進めています。

## 今できること

- カッコを含んだ四則演算
- 比較演算
- ローカル変数
- return文
- if文
- while文
- for文
- ブロック

詳しくは [`test.sh`](./test.sh) のテストケースを見ると良いかもしれません。

## 動かし方

WSL Ubuntu-24.04 の上で `clang` を使って開発しています。それ以外の環境だと上手く動かないかもしれないですが、適当に調整してください。

```
$ clang --version
Ubuntu clang version 18.1.3 (1)
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin
```

- ビルド `make`
- テスト `make test`
  - ビルドしてない場合は自動でビルドされます
- 環境の初期化 `make clean`
  - 全部のobjectファイルとかを消します
