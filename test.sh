#!/bin/bash

ok_count=0
ng_count=0

describe() {
  echo -e "\n$1"
}

assert() {
  col_red="\x1b[31m"
  col_green="\x1b[32m"
  col_yellow="\x1b[33m"
  col_blue="\x1b[34m"
  col_magenta="\x1b[35m"
  col_cyan="\x1b[36m"
  col_reset="\x1b[0m"

  ok="$col_green[OK]$col_reset"
  ng="$col_red[NG]$col_reset"

  input="$1"
  expected="$2"

  ./dist/1cc "$input" >./dist/tmp.ll
  if [ $? -ne 0 ]; then
    echo -e "$ng Failed to compile \"$input\""
    ng_count=$((ng_count + 1))
    return
  fi
  clang ./dist/tmp.ll -o ./dist/tmp
  actual=$(./dist/tmp)

  if [ "$actual" = "$expected" ]; then
    echo -e $ok "\"$input\"" "$col_yellow=>$col_reset" "\"$actual\""
    ok_count=$((ok_count + 1))
  else
    echo -e $ng "\"$input\"" "$col_yellow=>$col_reset" "\"$expected\""
    echo -e "    actual: \"$actual\""
    ng_count=$((ng_count + 1))
  fi
}

describe "単項"
assert "0;" "0"
assert "42;" "42"

describe "加減算"
assert "1 + 3;" "4"
assert "3 - 2;" "1"
assert "5 + 3 - 2;" "6"
assert "5 - 3 - 2;" "0"
assert "13 - 5 + 66;" "74"

describe "乗除算あり"
assert "2 * 3;" "6"
assert "6 / 2;" "3"
assert "2 * 3 + 4;" "10"
assert "2 + 3 * 4;" "14"
assert "2 * 3 * 4;" "24"
assert "2 * 3 / 4;" "1"
assert "2 / 3 * 4;" "0"

describe "括弧あり"
assert "(2 + 3) * 4;" "20"
assert "2 * (3 + 4);" "14"
assert "2 * (3 + 4) * 5;" "70"
assert "2 * (3 + 4) / 5;" "2"

describe "単項演算子"
assert "-1;" "-1"
assert "-(1 + 2);" "-3"
assert "-(1 + 2) * 3;" "-9"
assert "-(1 + 2) * -3;" "9"
assert "+4 * 3;" "12"
assert "3 * +2;" "6"
assert "+(-3 * 4);" "-12"

describe "比較演算子"
assert "3 == 3;" "1"
assert "3 == 2 + 1;" "1"
assert "3 == 2;" "0"
assert "4 - 1 == 2;" "0"
assert "3 != 4;" "1"
assert "3 != 3;" "0"
assert "3 != 2 + 1;" "0"
assert "3 < 5;" "1"
assert "6 < 5;" "0"
assert "5 < 5;" "0"
assert "3 <= 5;" "1"
assert "6 <= 5;" "0"
assert "5 <= 5;" "1"
assert "3 > 5;" "0"
assert "6 > 5;" "1"
assert "5 > 5;" "0"
assert "3 >= 5;" "0"
assert "6 >= 5;" "1"
assert "5 >= 5;" "1"

describe "ローカル変数"
assert "a = 3; a;" "3"
assert "aaa = 3; bbbb = 5; aaa + bbbb;" "8"
assert "a = 3; b = 5; a = 4; a + b;" "9"
assert "a = b = 3; a * 2 * b;" "18"

describe "return文"
assert "return 3;" "3"
assert "return 3; 5;" "3"
assert "return 3; return 5;" "3"
assert "a = 3; return a;" "3"
assert "a = 3; b = 5; return a + b;" "8"
assert "a = 3; b = 5; return a + b; return a * b;" "8"

describe "if文"
assert "if (1) return 3;" "3"
assert "if (0) return 3; return 5;" "5"
assert "if (1) return 3; return 5;" "3"
assert "if (1) return 3; else return 5;" "3"
assert "if (0) return 3; else return 5;" "5"
assert "if (1) return 3; else return 5; return 7;" "3"
assert "if (0) return 3; else return 5; return 7;" "5"
assert "if (1) return 3; else if (0) return 5; return 7;" "3"
assert "if (0) return 3; else if (1) return 5; return 7;" "5"
assert "if (0) return 3; else if (0) return 5; return 7;" "7"
assert "a = 3; if (a == 3) return 3; else return 7;" "3"
assert "a = 3; if (a > 5) return 3; else return 7;" "7"

describe "while文"
assert "a = 0; while (a < 3) a = a + 1; return a;" "3"
assert "a = 2; while (a < 16) a = a * 2; return a;" "16"
assert "a = 0; b = 1; while (a < 55) a = ((b = (a + b)) - a); return a;" "55"

describe "for文"
assert "a = 0; for (i = 0; i < 3; i = i + 1) a = a + 1; return a;" "3"
assert "a = 2; for (i = 0; i < 3; i = i + 1) a = a * 2; return a;" "16"
assert "a = 0; b = 1; for (i = 0; i < 10; i = i + 1) a = ((b = (a + b)) - a); return a;" "55"

if [ $ng_count -eq 0 ]; then
  echo -e "\n${col_green}all $ok_count tests passed🎉${col_reset}"
else
  echo -e "$col_green\n$ok_count tests passed🎉$col_reset"
  echo -e "$col_red$ng_count tests failed😭$col_reset"
fi
