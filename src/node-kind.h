#pragma once

// 抽象構文木のノードの種類
typedef enum {
  ND_ADD,     // +
  ND_SUB,     // -
  ND_MUL,     // *
  ND_DIV,     // /
  ND_EQ,      // ==
  ND_NE,      // !=
  ND_LT,      // <
  ND_LE,      // <=
  ND_GT,      // >
  ND_GE,      // >=
  ND_AND,     // &&
  ND_OR,      // ||
  ND_INCR,    // ++ (後ろに付く場合)
  ND_DECR,    // -- (後ろに付く場合)
  ND_REF,     // & (アドレス演算子)
  ND_DEREF,   // * (間接参照演算子)
  ND_ACCESS,  // . (構造体メンバアクセス演算子)
  ND_ASSIGN,  // = (代入)
  ND_LVAR,    // ローカル変数
  ND_RETURN,  // return
  ND_IF,      // if
  ND_WHILE,   // while
  ND_FOR,     // for
  ND_BLOCK,   // { ... }
  ND_GROUP,   // 複数のノードをまとめる
  ND_CALL,    // 関数呼び出し
  ND_NUM,     // 整数
} NodeKind;
