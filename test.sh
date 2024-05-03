#!/bin/bash
assert() {
  input="$1"
  expected="$2"

  ./dist/1cc "$input" > ./dist/tmp.ll
  clang ./dist/tmp.ll -o ./dist/tmp
  actual=$(./dist/tmp)

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected is expected, but got $actual"
    exit 1
  fi
}

# 単項
assert "0" "0"
assert "42" "42"

# 加減算のみ
assert "1 + 3" "4"
assert "3 - 2" "1"
assert "5 + 3 - 2" "6"
assert "13 - 5 + 66" "74"

# 乗除算あり
assert "2 * 3" "6"
assert "6 / 2" "3"
assert "2 * 3 + 4" "10"
assert "2 + 3 * 4" "14"
assert "2 * 3 * 4" "24"
assert "2 * 3 / 4" "1"
assert "2 / 3 * 4" "0"

# 括弧あり
assert "(2 + 3) * 4" "20"
assert "2 * (3 + 4)" "14"
assert "2 * (3 + 4) * 5" "70"
assert "2 * (3 + 4) / 5" "2"

# 単項演算子
assert "-1" "-1"
assert "-(1 + 2)" "-3"
assert "-(1 + 2) * 3" "-9"
assert "-(1 + 2) * -3" "9"
assert "+4 * 3" "12"
assert "3 * +2" "6"
assert "+(-3 * 4)" "-12"

echo OK