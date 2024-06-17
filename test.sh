#!/bin/bash

# テストの結果を格納するための配列
pids=()
test_names=()
result_files=()

current_test_name=""
describe() {
  current_test_name="$1"
}

assert() {
  col_red="\x1b[31m"
  col_green="\x1b[32m"
  col_yellow="\x1b[33m"
  col_blue="\x1b[34m"
  col_magenta="\x1b[35m"
  col_cyan="\x1b[36m"
  col_cyan="\x1b[36m"
  col_reset="\x1b[0m"

  ok="$col_green✅ [OK]$col_reset"
  ng="$col_red❌ [NG]$col_reset"

  input="$1"
  expected="$2"

  # Create a temporary file for storing the result
  result_file=$(mktemp)

  {
    tmp_ll_file="$(mktemp).ll"
    clang_output_object="$(mktemp).o"
    clang_output="$(mktemp)"
    compiler_stderr=$(./dist/1cc "$input" >"$tmp_ll_file" 2>&1)
    if [ $? -ne 0 ]; then
      echo -e "$ng \"$input\"" >"$result_file"
      echo -e "  ❌ Compile (1cc)" >>"$result_file"
      echo -e "" >>"$result_file"
      echo -e "$compiler_stderr" | awk '{print "  " $0}' >>"$result_file"
      echo -e "" >>"$result_file"
      exit 1
    fi
    clang_compile_stderr=$(clang -v "$tmp_ll_file" -c -o $clang_output_object 2>&1)
    if [ $? -ne 0 ]; then
      echo -e "$ng \"$input\"" >"$result_file"
      echo -e "  ✅ Compile (1cc)" "$col_yellow=>$col_reset" "$tmp_ll_file" >>"$result_file"
      echo -e "  ❌ Translate (clang)" >>"$result_file"
      echo -e "" >>"$result_file"
      echo -e "$clang_compile_stderr" | awk '{print "  " $0}' >>"$result_file"
      echo -e "" >>"$result_file"
      exit 1
    fi
    clang_link_stderr=$(clang -v "$clang_output_object" $(find . -wholename "./dist/lib/*.o") -o "$clang_output" 2>&1)
    if [ $? -ne 0 ]; then
      echo -e "$ng \"$input\"" >"$result_file"
      echo -e "  ✅ Compile (1cc)" "$col_yellow=>$col_reset" "$tmp_ll_file" >>"$result_file"
      echo -e "  ✅ Translate (clang)" "$col_yellow=>$col_reset" "$clang_output_object" >>"$result_file"
      echo -e "  ❌ Link" >>"$result_file"
      echo -e "" >>"$result_file"
      echo -e "$clang_link_stderr" | awk '{print "  " $0}' >>"$result_file"
      echo -e "" >>"$result_file"
      exit 1
    fi
    actual=$($clang_output)
    if [ $? -ne 0 ]; then
      echo -e "$ng \"$input\"" >"$result_file"
      echo -e "  ✅ Compile (1cc)" "$col_yellow=>$col_reset" "$tmp_ll_file" >>"$result_file"
      echo -e "  ✅ Translate (clang)" "$col_yellow=>$col_reset" "$clang_output_object" >>"$result_file"
      echo -e "  ✅ Link" "$col_yellow=>$col_reset" "$clang_output" >>"$result_file"
      echo -e "  ❌ Execute" >>"$result_file"
      echo -e "" >>"$result_file"
      exit 1
    fi

    # 改行を空白に直す
    actual=$(echo ${actual} | sed -e "s/[\r\n]\+//g")

    if [ "$actual" = "$expected" ]; then
      echo -e $ok "\"$input\"" "$col_yellow=>$col_reset" "\"$actual\"" >"$result_file"
      exit 0
    else
      echo -e $ng "\"$input\"" "$col_yellow=>$col_reset" "\"$expected\"" >>"$result_file"
      echo -e "  ✅ Compile (1cc)" "$col_yellow=>$col_reset" "$tmp_ll_file" >>"$result_file"
      echo -e "  ✅ Translate (clang)" "$col_yellow=>$col_reset" "$clang_output_object" >>"$result_file"
      echo -e "  ✅ Link" "$col_yellow=>$col_reset" "$clang_output" >>"$result_file"
      echo -e "  ✅ Execute" >>"$result_file"
      echo -e "  ❌ Result" "$col_yellow=>$col_reset" "got $actual" >>"$result_file"
      echo -e "" >>"$result_file"
      exit 1
    fi
    rm "$tmp_ll_file" "$clang_output"
  } &

  # Store the process ID and result file for later
  pids+=($!)
  test_names+=("$current_test_name")
  result_files+=("$result_file")
}

# テストの定義
describe "単項"
assert "int main() { print(0); }" "0"
assert "int main() { print(42); }" "42"

describe "加減算"
assert "int main() { print(1 + 3); }" "4"
assert "int main() { print(3 - 2); }" "1"
assert "int main() { print(5 + 3 - 2); }" "6"
assert "int main() { print(5 - 3 - 2); }" "0"
assert "int main() { print(13 - 5 + 66); }" "74"

describe "乗除算あり"
assert "int main() { print(2 * 3); }" "6"
assert "int main() { print(6 / 2); }" "3"
assert "int main() { print(2 * 3 + 4); }" "10"
assert "int main() { print(2 + 3 * 4); }" "14"
assert "int main() { print(2 * 3 * 4); }" "24"
assert "int main() { print(2 * 3 / 4); }" "1"
assert "int main() { print(2 / 3 * 4); }" "0"

describe "括弧あり"
assert "int main() { print((2 + 3) * 4); }" "20"
assert "int main() { print(2 * (3 + 4)); }" "14"
assert "int main() { print(2 * (3 + 4) * 5); }" "70"
assert "int main() { print(2 * (3 + 4) / 5); }" "2"

describe "単項演算子"
assert "int main() { print(-1); }" "-1"
assert "int main() { print(-(1 + 2)); }" "-3"
assert "int main() { print(-(1 + 2) * 3); }" "-9"
assert "int main() { print(-(1 + 2) * -3); }" "9"
assert "int main() { print(+4 * 3); }" "12"
assert "int main() { print(3 * +2); }" "6"
assert "int main() { print(+(-3 * 4)); }" "-12"

describe "比較演算子"
assert "int main() { print(3 == 3); }" "1"
assert "int main() { print(3 == 2 + 1); }" "1"
assert "int main() { print(3 == 2); }" "0"
assert "int main() { print(4 - 1 == 2); }" "0"
assert "int main() { print(3 != 4); }" "1"
assert "int main() { print(3 != 3); }" "0"
assert "int main() { print(3 != 2 + 1); }" "0"
assert "int main() { print(3 < 5); }" "1"
assert "int main() { print(6 < 5); }" "0"
assert "int main() { print(5 < 5); }" "0"
assert "int main() { print(3 <= 5); }" "1"
assert "int main() { print(6 <= 5); }" "0"
assert "int main() { print(5 <= 5); }" "1"
assert "int main() { print(3 > 5); }" "0"
assert "int main() { print(6 > 5); }" "1"
assert "int main() { print(5 > 5); }" "0"
assert "int main() { print(3 >= 5); }" "0"
assert "int main() { print(6 >= 5); }" "1"
assert "int main() { print(5 >= 5); }" "1"

describe "否定演算子"
assert "int main() { print(!1); }" "0"
assert "int main() { print(!3); }" "0"
assert "int main() { print(!0); }" "1"

describe "論理演算子"
assert "int main() { print(1 && 1); }" "1"
assert "int main() { print(1 && 0); }" "0"
assert "int main() { print(0 && 1); }" "0"
assert "int main() { print(0 && 0); }" "0"
assert "int main() { print(1 || 1); }" "1"
assert "int main() { print(1 || 0); }" "1"
assert "int main() { print(0 || 1); }" "1"
assert "int main() { print(0 || 0); }" "0"

describe "インクリメント / デクリメント"
assert "int main() { int a = 3; print(a++); print(a); }" "3 4"
assert "int main() { int a = 3; print(a--); print(a); }" "3 2"
assert "int main() { int a = 3; print(++a); print(a); }" "4 4"
assert "int main() { int a = 3; print(--a); print(a); }" "2 2"

describe "ローカル変数"
assert "int main() { int a = 3; print(a); }" "3"
assert "int main() { int aaa = 3; int bbbb = 5; print(aaa + bbbb); }" "8"
assert "int main() { int a = 4, b = 5; print(a + b); }" "9"
assert "int main() { int a, b; a = b = 3; print(a * 2 * b); }" "18"

describe "return文"
assert "int main() { print(func()); } int func() { return 3; }" "3"
assert "int main() { print(func()); } int func() { return 3; return 5; }" "3"
assert "int main() { print(func()); } int func() { return 3; return 5; }" "3"
assert "int main() { print(func()); } int func() { int a = 3; return a; }" "3"
assert "int main() { print(func()); } int func() { int a = 3, b = 5; return a + b; }" "8"
assert "int main() { print(func()); } int func() { int a = 3, b = 5; return a + b; return a * b; }" "8"

describe "if文"
assert "int main() { if (1) print(3); }" "3"
assert "int main() { if (0) print(3); print(5); }" "5"
assert "int main() { if (1) print(3); else print(5); }" "3"
assert "int main() { if (0) print(3); else print(5); }" "5"
assert "int main() { if (1) print(3); else if (0) print(5); else print(8); }" "3"
assert "int main() { if (0) print(3); else if (1) print(5); else print(8); }" "5"
assert "int main() { if (0) print(3); else if (0) print(5); else print(8); }" "8"
assert "int main() { if (1) print(3); else if (1) print(5); else print(8); }" "3"

describe "while文"
assert "int main() { int a = 0; while (a < 3) a = a + 1; print(a); }" "3"
assert "int main() { int a = 2; while (a < 16) a = a * 2; print(a); }" "16"
assert "int main() { int a = 0, b = 1; while (a < 55) a = ((b = (a + b)) - a); print(a); }" "55"

describe "for文"
assert "int main() { int a = 0; for (int i = 0; i < 3; i = i + 1) a = a + 1; print(a); }" "3"
assert "int main() { int a = 2; for (int i = 0; i < 3; i = i + 1) a = a * 2; print(a); }" "16"
assert "int main() { int a = 0, b = 1; for (int i = 0; i < 10; i = i + 1) a = ((b = (a + b)) - a); print(a); }" "55"

describe "ブロック"
assert "int main() { { print(3); } }" "3"
assert "int main() { { int a = 3; print(a); } }" "3"
assert "int main() { int a = 0, b = 1; for (int i = 0; i < 10; i = i + 1) { int tmp = b; b = a + b; a = tmp; } print(a); }" "55"

describe "関数"
assert "int main() { print(func()); } int func() { return 3; }" "3"
assert "int main() { print(func(3)); } int func(int a) { return a; }" "3"
assert "int main() { print(func(3, 5)); } int func(int a, int b) { return a + b; }" "8"
assert "int main() { print(func(add1(3), sub1(5))); } int func(int a, int b) { return a * b; } int add1(int a) { return a + 1; } int sub1(int a) { return a - 1; }" "16"
assert "int main() { print(fib(10)); } int fib(int n) { if (n <= 1) return n; return fib(n - 1) + fib(n - 2); }" "55"

describe "ポインタ"
assert "int main() { int a = 3; int *b = &a; print(*b); }" "3"
assert "int main() { int a = 3; int *b = &a; *b = 5; print(a); }" "5"
assert "int main() { int a = 3; int *b = &a; int **c = &b; print(**c); }" "3"
assert "int main() { int a = 3; int *b = &a; int **c = &b; **c = 5; print(a); }" "5"
assert "int main() { int *p; alloc4(&p, 1, 2, 4, 8); int *q = p + 2; print(*q); q = q + 1; print(*q); q = q - 2; print(*q); }" "4 8 2"

# 全てのテストが完了するのを待つ
for pid in "${pids[@]}"; do
  wait "$pid"
done

# 結果を表示する
ok_count=0
ng_count=0
current_test_name=""
for i in $(seq 0 $((${#result_files[@]} - 1))); do
  result_file="${result_files[$i]}"
  test_name="${test_names[$i]}"
  if [ "$current_test_name" != "$test_name" ]; then
    echo -e "\n$test_name"
    current_test_name="$test_name"
  fi
  result=$(cat "$result_file")
  echo -e "$result"
  if [[ "$result" =~ \[NG\] ]]; then
    ng_count=$((ng_count + 1))
  else
    ok_count=$((ok_count + 1))
  fi
  rm "$result_file"
done

if [ $ng_count -eq 0 ]; then
  echo -e "\n${col_green}all $ok_count tests passed🎉${col_reset}"
else
  echo -e "$col_green\n$ok_count tests passed🎉$col_reset"
  echo -e "$col_red$ng_count tests failed😭$col_reset"
fi
