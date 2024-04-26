#!/bin/bash
assert() {
  input="$1"
  expected="$2"

  ./dist/calc "$input" > ./dist/tmp.ll
  clang ./dist/tmp.ll -o ./dist/tmp
  actual=$(./dist/tmp)

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected is expected, but got $actual"
    exit 1
  fi
}

assert "0" "0"
assert "42" "42"
assert "1 + 3" "4"
assert "3 - 2" "1"
assert "5 + 3 - 2" "6"
assert "13 - 5 + 66" "74"

echo OK