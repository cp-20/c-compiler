; ModuleID = 'src/tokenizer.c'
source_filename = "src/tokenizer.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.Token = type { i32, ptr, i32, ptr, i32 }

@.str = private unnamed_addr constant [3 x i8] c"==\00", align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"!=\00", align 1
@.str.2 = private unnamed_addr constant [3 x i8] c"<=\00", align 1
@.str.3 = private unnamed_addr constant [3 x i8] c">=\00", align 1
@.str.4 = private unnamed_addr constant [3 x i8] c"&&\00", align 1
@.str.5 = private unnamed_addr constant [3 x i8] c"||\00", align 1
@.str.6 = private unnamed_addr constant [3 x i8] c"++\00", align 1
@.str.7 = private unnamed_addr constant [3 x i8] c"--\00", align 1
@.str.8 = private unnamed_addr constant [3 x i8] c"->\00", align 1
@.str.9 = private unnamed_addr constant [3 x i8] c"+=\00", align 1
@.str.10 = private unnamed_addr constant [3 x i8] c"-=\00", align 1
@.str.11 = private unnamed_addr constant [3 x i8] c"*=\00", align 1
@.str.12 = private unnamed_addr constant [3 x i8] c"/=\00", align 1
@.str.13 = private unnamed_addr constant [4 x i8] c"...\00", align 1
@.str.14 = private unnamed_addr constant [3 x i8] c"//\00", align 1
@.str.15 = private unnamed_addr constant [3 x i8] c"/*\00", align 1
@.str.16 = private unnamed_addr constant [3 x i8] c"*/\00", align 1
@.str.17 = private unnamed_addr constant [43 x i8] c"\E3\82\B3\E3\83\A1\E3\83\B3\E3\83\88\E3\81\8C\E9\96\89\E3\81\98\E3\82\89\E3\82\8C\E3\81\A6\E3\81\84\E3\81\BE\E3\81\9B\E3\82\93\00", align 1
@.str.18 = private unnamed_addr constant [7 x i8] c"return\00", align 1
@.str.19 = private unnamed_addr constant [3 x i8] c"if\00", align 1
@.str.20 = private unnamed_addr constant [5 x i8] c"else\00", align 1
@.str.21 = private unnamed_addr constant [6 x i8] c"while\00", align 1
@.str.22 = private unnamed_addr constant [3 x i8] c"do\00", align 1
@.str.23 = private unnamed_addr constant [4 x i8] c"for\00", align 1
@.str.24 = private unnamed_addr constant [9 x i8] c"continue\00", align 1
@.str.25 = private unnamed_addr constant [6 x i8] c"break\00", align 1
@.str.26 = private unnamed_addr constant [7 x i8] c"switch\00", align 1
@.str.27 = private unnamed_addr constant [5 x i8] c"case\00", align 1
@.str.28 = private unnamed_addr constant [8 x i8] c"default\00", align 1
@.str.29 = private unnamed_addr constant [4 x i8] c"int\00", align 1
@.str.30 = private unnamed_addr constant [5 x i8] c"char\00", align 1
@.str.31 = private unnamed_addr constant [5 x i8] c"void\00", align 1
@.str.32 = private unnamed_addr constant [7 x i8] c"struct\00", align 1
@.str.33 = private unnamed_addr constant [7 x i8] c"sizeof\00", align 1
@.str.34 = private unnamed_addr constant [5 x i8] c"enum\00", align 1
@.str.35 = private unnamed_addr constant [8 x i8] c"typedef\00", align 1
@.str.36 = private unnamed_addr constant [7 x i8] c"extern\00", align 1
@.str.37 = private unnamed_addr constant [25 x i8] c"\E6\96\87\E5\AD\97\E3\81\8C\E3\81\82\E3\82\8A\E3\81\BE\E3\81\9B\E3\82\93\00", align 1
@.str.38 = private unnamed_addr constant [25 x i8] c"\E6\96\87\E5\AD\97\E3\81\8C\E9\95\B7\E3\81\99\E3\81\8E\E3\81\BE\E3\81\99\00", align 1
@.str.39 = private unnamed_addr constant [40 x i8] c"\E6\96\87\E5\AD\97\E5\88\97\E3\81\8C\E9\96\89\E3\81\98\E3\82\89\E3\82\8C\E3\81\A6\E3\81\84\E3\81\BE\E3\81\9B\E3\82\93\00", align 1
@.str.40 = private unnamed_addr constant [34 x i8] c"\E3\83\88\E3\83\BC\E3\82\AF\E3\83\8A\E3\82\A4\E3\82\BA\E3\81\A7\E3\81\8D\E3\81\BE\E3\81\9B\E3\82\93\00", align 1
@.str.41 = private unnamed_addr constant [31 x i8] c"\E4\BA\88\E7\B4\84\E8\AA\9E\E3\81\A7\E3\81\AF\E3\81\82\E3\82\8A\E3\81\BE\E3\81\9B\E3\82\93\00", align 1
@.str.42 = private unnamed_addr constant [26 x i8] c"'%s'\E3\81\A7\E3\81\AF\E3\81\82\E3\82\8A\E3\81\BE\E3\81\9B\E3\82\93\00", align 1
@.str.43 = private unnamed_addr constant [25 x i8] c"\E6\95\B0\E3\81\A7\E3\81\AF\E3\81\82\E3\82\8A\E3\81\BE\E3\81\9B\E3\82\93\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @new_token(i32 noundef %0, ptr noundef %1, ptr noundef %2) #0 {
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca ptr, align 8
  store i32 %0, ptr %4, align 4
  store ptr %1, ptr %5, align 8
  store ptr %2, ptr %6, align 8
  %8 = call noalias ptr @calloc(i64 noundef 1, i64 noundef 40) #5
  store ptr %8, ptr %7, align 8
  %9 = load i32, ptr %4, align 4
  %10 = load ptr, ptr %7, align 8
  %11 = getelementptr inbounds %struct.Token, ptr %10, i32 0, i32 0
  store i32 %9, ptr %11, align 8
  %12 = load ptr, ptr %6, align 8
  %13 = load ptr, ptr %7, align 8
  %14 = getelementptr inbounds %struct.Token, ptr %13, i32 0, i32 3
  store ptr %12, ptr %14, align 8
  %15 = load ptr, ptr %7, align 8
  %16 = load ptr, ptr %5, align 8
  %17 = getelementptr inbounds %struct.Token, ptr %16, i32 0, i32 1
  store ptr %15, ptr %17, align 8
  %18 = load ptr, ptr %7, align 8
  ret ptr %18
}

; Function Attrs: nounwind allocsize(0,1)
declare noalias ptr @calloc(i64 noundef, i64 noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i1 @is_space(i8 noundef signext %0) #0 {
  %2 = alloca i1, align 1
  %3 = alloca i8, align 1
  store i8 %0, ptr %3, align 1
  %4 = load i8, ptr %3, align 1
  %5 = sext i8 %4 to i32
  %6 = icmp eq i32 %5, 32
  br i1 %6, label %7, label %8

7:                                                ; preds = %1
  store i1 true, ptr %2, align 1
  br label %24

8:                                                ; preds = %1
  %9 = load i8, ptr %3, align 1
  %10 = sext i8 %9 to i32
  %11 = icmp eq i32 %10, 9
  br i1 %11, label %12, label %13

12:                                               ; preds = %8
  store i1 true, ptr %2, align 1
  br label %24

13:                                               ; preds = %8
  %14 = load i8, ptr %3, align 1
  %15 = sext i8 %14 to i32
  %16 = icmp eq i32 %15, 10
  br i1 %16, label %17, label %18

17:                                               ; preds = %13
  store i1 true, ptr %2, align 1
  br label %24

18:                                               ; preds = %13
  %19 = load i8, ptr %3, align 1
  %20 = sext i8 %19 to i32
  %21 = icmp eq i32 %20, 13
  br i1 %21, label %22, label %23

22:                                               ; preds = %18
  store i1 true, ptr %2, align 1
  br label %24

23:                                               ; preds = %18
  store i1 false, ptr %2, align 1
  br label %24

24:                                               ; preds = %23, %22, %17, %12, %7
  %25 = load i1, ptr %2, align 1
  ret i1 %25
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i1 @is_num(i8 noundef signext %0) #0 {
  %2 = alloca i8, align 1
  store i8 %0, ptr %2, align 1
  %3 = load i8, ptr %2, align 1
  %4 = sext i8 %3 to i32
  %5 = icmp sle i32 48, %4
  br i1 %5, label %6, label %10

6:                                                ; preds = %1
  %7 = load i8, ptr %2, align 1
  %8 = sext i8 %7 to i32
  %9 = icmp sle i32 %8, 57
  br label %10

10:                                               ; preds = %6, %1
  %11 = phi i1 [ false, %1 ], [ %9, %6 ]
  ret i1 %11
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i1 @is_alpha(i8 noundef signext %0) #0 {
  %2 = alloca i1, align 1
  %3 = alloca i8, align 1
  store i8 %0, ptr %3, align 1
  %4 = load i8, ptr %3, align 1
  %5 = sext i8 %4 to i32
  %6 = icmp sle i32 97, %5
  br i1 %6, label %7, label %12

7:                                                ; preds = %1
  %8 = load i8, ptr %3, align 1
  %9 = sext i8 %8 to i32
  %10 = icmp sle i32 %9, 122
  br i1 %10, label %11, label %12

11:                                               ; preds = %7
  store i1 true, ptr %2, align 1
  br label %27

12:                                               ; preds = %7, %1
  %13 = load i8, ptr %3, align 1
  %14 = sext i8 %13 to i32
  %15 = icmp sle i32 65, %14
  br i1 %15, label %16, label %21

16:                                               ; preds = %12
  %17 = load i8, ptr %3, align 1
  %18 = sext i8 %17 to i32
  %19 = icmp sle i32 %18, 90
  br i1 %19, label %20, label %21

20:                                               ; preds = %16
  store i1 true, ptr %2, align 1
  br label %27

21:                                               ; preds = %16, %12
  %22 = load i8, ptr %3, align 1
  %23 = sext i8 %22 to i32
  %24 = icmp eq i32 %23, 95
  br i1 %24, label %25, label %26

25:                                               ; preds = %21
  store i1 true, ptr %2, align 1
  br label %27

26:                                               ; preds = %21
  store i1 false, ptr %2, align 1
  br label %27

27:                                               ; preds = %26, %25, %20, %11
  %28 = load i1, ptr %2, align 1
  ret i1 %28
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i1 @is_special(i8 noundef signext %0) #0 {
  %2 = alloca i1, align 1
  %3 = alloca i8, align 1
  store i8 %0, ptr %3, align 1
  %4 = load i8, ptr %3, align 1
  %5 = sext i8 %4 to i32
  %6 = icmp eq i32 %5, 43
  br i1 %6, label %7, label %8

7:                                                ; preds = %1
  store i1 true, ptr %2, align 1
  br label %104

8:                                                ; preds = %1
  %9 = load i8, ptr %3, align 1
  %10 = sext i8 %9 to i32
  %11 = icmp eq i32 %10, 45
  br i1 %11, label %12, label %13

12:                                               ; preds = %8
  store i1 true, ptr %2, align 1
  br label %104

13:                                               ; preds = %8
  %14 = load i8, ptr %3, align 1
  %15 = sext i8 %14 to i32
  %16 = icmp eq i32 %15, 42
  br i1 %16, label %17, label %18

17:                                               ; preds = %13
  store i1 true, ptr %2, align 1
  br label %104

18:                                               ; preds = %13
  %19 = load i8, ptr %3, align 1
  %20 = sext i8 %19 to i32
  %21 = icmp eq i32 %20, 47
  br i1 %21, label %22, label %23

22:                                               ; preds = %18
  store i1 true, ptr %2, align 1
  br label %104

23:                                               ; preds = %18
  %24 = load i8, ptr %3, align 1
  %25 = sext i8 %24 to i32
  %26 = icmp eq i32 %25, 40
  br i1 %26, label %27, label %28

27:                                               ; preds = %23
  store i1 true, ptr %2, align 1
  br label %104

28:                                               ; preds = %23
  %29 = load i8, ptr %3, align 1
  %30 = sext i8 %29 to i32
  %31 = icmp eq i32 %30, 41
  br i1 %31, label %32, label %33

32:                                               ; preds = %28
  store i1 true, ptr %2, align 1
  br label %104

33:                                               ; preds = %28
  %34 = load i8, ptr %3, align 1
  %35 = sext i8 %34 to i32
  %36 = icmp eq i32 %35, 60
  br i1 %36, label %37, label %38

37:                                               ; preds = %33
  store i1 true, ptr %2, align 1
  br label %104

38:                                               ; preds = %33
  %39 = load i8, ptr %3, align 1
  %40 = sext i8 %39 to i32
  %41 = icmp eq i32 %40, 62
  br i1 %41, label %42, label %43

42:                                               ; preds = %38
  store i1 true, ptr %2, align 1
  br label %104

43:                                               ; preds = %38
  %44 = load i8, ptr %3, align 1
  %45 = sext i8 %44 to i32
  %46 = icmp eq i32 %45, 61
  br i1 %46, label %47, label %48

47:                                               ; preds = %43
  store i1 true, ptr %2, align 1
  br label %104

48:                                               ; preds = %43
  %49 = load i8, ptr %3, align 1
  %50 = sext i8 %49 to i32
  %51 = icmp eq i32 %50, 59
  br i1 %51, label %52, label %53

52:                                               ; preds = %48
  store i1 true, ptr %2, align 1
  br label %104

53:                                               ; preds = %48
  %54 = load i8, ptr %3, align 1
  %55 = sext i8 %54 to i32
  %56 = icmp eq i32 %55, 123
  br i1 %56, label %57, label %58

57:                                               ; preds = %53
  store i1 true, ptr %2, align 1
  br label %104

58:                                               ; preds = %53
  %59 = load i8, ptr %3, align 1
  %60 = sext i8 %59 to i32
  %61 = icmp eq i32 %60, 125
  br i1 %61, label %62, label %63

62:                                               ; preds = %58
  store i1 true, ptr %2, align 1
  br label %104

63:                                               ; preds = %58
  %64 = load i8, ptr %3, align 1
  %65 = sext i8 %64 to i32
  %66 = icmp eq i32 %65, 44
  br i1 %66, label %67, label %68

67:                                               ; preds = %63
  store i1 true, ptr %2, align 1
  br label %104

68:                                               ; preds = %63
  %69 = load i8, ptr %3, align 1
  %70 = sext i8 %69 to i32
  %71 = icmp eq i32 %70, 33
  br i1 %71, label %72, label %73

72:                                               ; preds = %68
  store i1 true, ptr %2, align 1
  br label %104

73:                                               ; preds = %68
  %74 = load i8, ptr %3, align 1
  %75 = sext i8 %74 to i32
  %76 = icmp eq i32 %75, 38
  br i1 %76, label %77, label %78

77:                                               ; preds = %73
  store i1 true, ptr %2, align 1
  br label %104

78:                                               ; preds = %73
  %79 = load i8, ptr %3, align 1
  %80 = sext i8 %79 to i32
  %81 = icmp eq i32 %80, 91
  br i1 %81, label %82, label %83

82:                                               ; preds = %78
  store i1 true, ptr %2, align 1
  br label %104

83:                                               ; preds = %78
  %84 = load i8, ptr %3, align 1
  %85 = sext i8 %84 to i32
  %86 = icmp eq i32 %85, 93
  br i1 %86, label %87, label %88

87:                                               ; preds = %83
  store i1 true, ptr %2, align 1
  br label %104

88:                                               ; preds = %83
  %89 = load i8, ptr %3, align 1
  %90 = sext i8 %89 to i32
  %91 = icmp eq i32 %90, 46
  br i1 %91, label %92, label %93

92:                                               ; preds = %88
  store i1 true, ptr %2, align 1
  br label %104

93:                                               ; preds = %88
  %94 = load i8, ptr %3, align 1
  %95 = sext i8 %94 to i32
  %96 = icmp eq i32 %95, 63
  br i1 %96, label %97, label %98

97:                                               ; preds = %93
  store i1 true, ptr %2, align 1
  br label %104

98:                                               ; preds = %93
  %99 = load i8, ptr %3, align 1
  %100 = sext i8 %99 to i32
  %101 = icmp eq i32 %100, 58
  br i1 %101, label %102, label %103

102:                                              ; preds = %98
  store i1 true, ptr %2, align 1
  br label %104

103:                                              ; preds = %98
  store i1 false, ptr %2, align 1
  br label %104

104:                                              ; preds = %103, %102, %97, %92, %87, %82, %77, %72, %67, %62, %57, %52, %47, %42, %37, %32, %27, %22, %17, %12, %7
  %105 = load i1, ptr %2, align 1
  ret i1 %105
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i1 @is_special2(ptr noundef %0) #0 {
  %2 = alloca i1, align 1
  %3 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  %4 = load ptr, ptr %3, align 8
  %5 = call i32 @strncmp(ptr noundef %4, ptr noundef @.str, i64 noundef 2) #6
  %6 = icmp eq i32 %5, 0
  br i1 %6, label %7, label %8

7:                                                ; preds = %1
  store i1 true, ptr %2, align 1
  br label %69

8:                                                ; preds = %1
  %9 = load ptr, ptr %3, align 8
  %10 = call i32 @strncmp(ptr noundef %9, ptr noundef @.str.1, i64 noundef 2) #6
  %11 = icmp eq i32 %10, 0
  br i1 %11, label %12, label %13

12:                                               ; preds = %8
  store i1 true, ptr %2, align 1
  br label %69

13:                                               ; preds = %8
  %14 = load ptr, ptr %3, align 8
  %15 = call i32 @strncmp(ptr noundef %14, ptr noundef @.str.2, i64 noundef 2) #6
  %16 = icmp eq i32 %15, 0
  br i1 %16, label %17, label %18

17:                                               ; preds = %13
  store i1 true, ptr %2, align 1
  br label %69

18:                                               ; preds = %13
  %19 = load ptr, ptr %3, align 8
  %20 = call i32 @strncmp(ptr noundef %19, ptr noundef @.str.3, i64 noundef 2) #6
  %21 = icmp eq i32 %20, 0
  br i1 %21, label %22, label %23

22:                                               ; preds = %18
  store i1 true, ptr %2, align 1
  br label %69

23:                                               ; preds = %18
  %24 = load ptr, ptr %3, align 8
  %25 = call i32 @strncmp(ptr noundef %24, ptr noundef @.str.4, i64 noundef 2) #6
  %26 = icmp eq i32 %25, 0
  br i1 %26, label %27, label %28

27:                                               ; preds = %23
  store i1 true, ptr %2, align 1
  br label %69

28:                                               ; preds = %23
  %29 = load ptr, ptr %3, align 8
  %30 = call i32 @strncmp(ptr noundef %29, ptr noundef @.str.5, i64 noundef 2) #6
  %31 = icmp eq i32 %30, 0
  br i1 %31, label %32, label %33

32:                                               ; preds = %28
  store i1 true, ptr %2, align 1
  br label %69

33:                                               ; preds = %28
  %34 = load ptr, ptr %3, align 8
  %35 = call i32 @strncmp(ptr noundef %34, ptr noundef @.str.6, i64 noundef 2) #6
  %36 = icmp eq i32 %35, 0
  br i1 %36, label %37, label %38

37:                                               ; preds = %33
  store i1 true, ptr %2, align 1
  br label %69

38:                                               ; preds = %33
  %39 = load ptr, ptr %3, align 8
  %40 = call i32 @strncmp(ptr noundef %39, ptr noundef @.str.7, i64 noundef 2) #6
  %41 = icmp eq i32 %40, 0
  br i1 %41, label %42, label %43

42:                                               ; preds = %38
  store i1 true, ptr %2, align 1
  br label %69

43:                                               ; preds = %38
  %44 = load ptr, ptr %3, align 8
  %45 = call i32 @strncmp(ptr noundef %44, ptr noundef @.str.8, i64 noundef 2) #6
  %46 = icmp eq i32 %45, 0
  br i1 %46, label %47, label %48

47:                                               ; preds = %43
  store i1 true, ptr %2, align 1
  br label %69

48:                                               ; preds = %43
  %49 = load ptr, ptr %3, align 8
  %50 = call i32 @strncmp(ptr noundef %49, ptr noundef @.str.9, i64 noundef 2) #6
  %51 = icmp eq i32 %50, 0
  br i1 %51, label %52, label %53

52:                                               ; preds = %48
  store i1 true, ptr %2, align 1
  br label %69

53:                                               ; preds = %48
  %54 = load ptr, ptr %3, align 8
  %55 = call i32 @strncmp(ptr noundef %54, ptr noundef @.str.10, i64 noundef 2) #6
  %56 = icmp eq i32 %55, 0
  br i1 %56, label %57, label %58

57:                                               ; preds = %53
  store i1 true, ptr %2, align 1
  br label %69

58:                                               ; preds = %53
  %59 = load ptr, ptr %3, align 8
  %60 = call i32 @strncmp(ptr noundef %59, ptr noundef @.str.11, i64 noundef 2) #6
  %61 = icmp eq i32 %60, 0
  br i1 %61, label %62, label %63

62:                                               ; preds = %58
  store i1 true, ptr %2, align 1
  br label %69

63:                                               ; preds = %58
  %64 = load ptr, ptr %3, align 8
  %65 = call i32 @strncmp(ptr noundef %64, ptr noundef @.str.12, i64 noundef 2) #6
  %66 = icmp eq i32 %65, 0
  br i1 %66, label %67, label %68

67:                                               ; preds = %63
  store i1 true, ptr %2, align 1
  br label %69

68:                                               ; preds = %63
  store i1 false, ptr %2, align 1
  br label %69

69:                                               ; preds = %68, %67, %62, %57, %52, %47, %42, %37, %32, %27, %22, %17, %12, %7
  %70 = load i1, ptr %2, align 1
  ret i1 %70
}

; Function Attrs: nounwind willreturn memory(read)
declare i32 @strncmp(ptr noundef, ptr noundef, i64 noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i1 @is_special3(ptr noundef %0) #0 {
  %2 = alloca i1, align 1
  %3 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  %4 = load ptr, ptr %3, align 8
  %5 = call i32 @strncmp(ptr noundef %4, ptr noundef @.str.13, i64 noundef 3) #6
  %6 = icmp eq i32 %5, 0
  br i1 %6, label %7, label %8

7:                                                ; preds = %1
  store i1 true, ptr %2, align 1
  br label %9

8:                                                ; preds = %1
  store i1 false, ptr %2, align 1
  br label %9

9:                                                ; preds = %8, %7
  %10 = load i1, ptr %2, align 1
  ret i1 %10
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @tokenize(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca %struct.Token, align 8
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca ptr, align 8
  %8 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  %9 = getelementptr inbounds %struct.Token, ptr %3, i32 0, i32 1
  store ptr null, ptr %9, align 8
  store ptr %3, ptr %4, align 8
  br label %10

10:                                               ; preds = %600, %586, %541, %500, %484, %459, %439, %419, %399, %379, %359, %339, %319, %299, %279, %259, %239, %219, %199, %179, %159, %139, %119, %99, %78, %66, %55, %49, %36, %18, %1
  %11 = load ptr, ptr %2, align 8
  %12 = load i8, ptr %11, align 1
  %13 = icmp ne i8 %12, 0
  br i1 %13, label %14, label %602

14:                                               ; preds = %10
  %15 = load ptr, ptr %2, align 8
  %16 = load i8, ptr %15, align 1
  %17 = call zeroext i1 @is_space(i8 noundef signext %16)
  br i1 %17, label %18, label %21

18:                                               ; preds = %14
  %19 = load ptr, ptr %2, align 8
  %20 = getelementptr inbounds i8, ptr %19, i32 1
  store ptr %20, ptr %2, align 8
  br label %10, !llvm.loop !6

21:                                               ; preds = %14
  %22 = load ptr, ptr %2, align 8
  %23 = call i32 @strncmp(ptr noundef %22, ptr noundef @.str.14, i64 noundef 2) #6
  %24 = icmp eq i32 %23, 0
  br i1 %24, label %25, label %37

25:                                               ; preds = %21
  %26 = load ptr, ptr %2, align 8
  %27 = getelementptr inbounds i8, ptr %26, i64 2
  store ptr %27, ptr %2, align 8
  br label %28

28:                                               ; preds = %33, %25
  %29 = load ptr, ptr %2, align 8
  %30 = load i8, ptr %29, align 1
  %31 = sext i8 %30 to i32
  %32 = icmp ne i32 %31, 10
  br i1 %32, label %33, label %36

33:                                               ; preds = %28
  %34 = load ptr, ptr %2, align 8
  %35 = getelementptr inbounds i8, ptr %34, i32 1
  store ptr %35, ptr %2, align 8
  br label %28, !llvm.loop !8

36:                                               ; preds = %28
  br label %10, !llvm.loop !6

37:                                               ; preds = %21
  %38 = load ptr, ptr %2, align 8
  %39 = call i32 @strncmp(ptr noundef %38, ptr noundef @.str.15, i64 noundef 2) #6
  %40 = icmp eq i32 %39, 0
  br i1 %40, label %41, label %52

41:                                               ; preds = %37
  %42 = load ptr, ptr %2, align 8
  %43 = getelementptr inbounds i8, ptr %42, i64 2
  %44 = call ptr @strstr(ptr noundef %43, ptr noundef @.str.16) #6
  store ptr %44, ptr %5, align 8
  %45 = load ptr, ptr %5, align 8
  %46 = icmp ne ptr %45, null
  br i1 %46, label %49, label %47

47:                                               ; preds = %41
  %48 = load ptr, ptr %2, align 8
  call void (ptr, ptr, ...) @error_at(ptr noundef %48, ptr noundef @.str.17)
  br label %49

49:                                               ; preds = %47, %41
  %50 = load ptr, ptr %5, align 8
  %51 = getelementptr inbounds i8, ptr %50, i64 2
  store ptr %51, ptr %2, align 8
  br label %10, !llvm.loop !6

52:                                               ; preds = %37
  %53 = load ptr, ptr %2, align 8
  %54 = call zeroext i1 @is_special3(ptr noundef %53)
  br i1 %54, label %55, label %63

55:                                               ; preds = %52
  %56 = load ptr, ptr %4, align 8
  %57 = load ptr, ptr %2, align 8
  %58 = call ptr @new_token(i32 noundef 0, ptr noundef %56, ptr noundef %57)
  store ptr %58, ptr %4, align 8
  %59 = load ptr, ptr %4, align 8
  %60 = getelementptr inbounds %struct.Token, ptr %59, i32 0, i32 4
  store i32 3, ptr %60, align 8
  %61 = load ptr, ptr %2, align 8
  %62 = getelementptr inbounds i8, ptr %61, i64 3
  store ptr %62, ptr %2, align 8
  br label %10, !llvm.loop !6

63:                                               ; preds = %52
  %64 = load ptr, ptr %2, align 8
  %65 = call zeroext i1 @is_special2(ptr noundef %64)
  br i1 %65, label %66, label %74

66:                                               ; preds = %63
  %67 = load ptr, ptr %4, align 8
  %68 = load ptr, ptr %2, align 8
  %69 = call ptr @new_token(i32 noundef 0, ptr noundef %67, ptr noundef %68)
  store ptr %69, ptr %4, align 8
  %70 = load ptr, ptr %4, align 8
  %71 = getelementptr inbounds %struct.Token, ptr %70, i32 0, i32 4
  store i32 2, ptr %71, align 8
  %72 = load ptr, ptr %2, align 8
  %73 = getelementptr inbounds i8, ptr %72, i64 2
  store ptr %73, ptr %2, align 8
  br label %10, !llvm.loop !6

74:                                               ; preds = %63
  %75 = load ptr, ptr %2, align 8
  %76 = load i8, ptr %75, align 1
  %77 = call zeroext i1 @is_special(i8 noundef signext %76)
  br i1 %77, label %78, label %85

78:                                               ; preds = %74
  %79 = load ptr, ptr %4, align 8
  %80 = load ptr, ptr %2, align 8
  %81 = getelementptr inbounds i8, ptr %80, i32 1
  store ptr %81, ptr %2, align 8
  %82 = call ptr @new_token(i32 noundef 0, ptr noundef %79, ptr noundef %80)
  store ptr %82, ptr %4, align 8
  %83 = load ptr, ptr %4, align 8
  %84 = getelementptr inbounds %struct.Token, ptr %83, i32 0, i32 4
  store i32 1, ptr %84, align 8
  br label %10, !llvm.loop !6

85:                                               ; preds = %74
  %86 = load ptr, ptr %2, align 8
  %87 = call i32 @strncmp(ptr noundef %86, ptr noundef @.str.18, i64 noundef 6) #6
  %88 = icmp eq i32 %87, 0
  br i1 %88, label %89, label %105

89:                                               ; preds = %85
  %90 = load ptr, ptr %2, align 8
  %91 = getelementptr inbounds i8, ptr %90, i64 6
  %92 = load i8, ptr %91, align 1
  %93 = call zeroext i1 @is_alpha(i8 noundef signext %92)
  br i1 %93, label %105, label %94

94:                                               ; preds = %89
  %95 = load ptr, ptr %2, align 8
  %96 = getelementptr inbounds i8, ptr %95, i64 6
  %97 = load i8, ptr %96, align 1
  %98 = call zeroext i1 @is_num(i8 noundef signext %97)
  br i1 %98, label %105, label %99

99:                                               ; preds = %94
  %100 = load ptr, ptr %4, align 8
  %101 = load ptr, ptr %2, align 8
  %102 = call ptr @new_token(i32 noundef 4, ptr noundef %100, ptr noundef %101)
  store ptr %102, ptr %4, align 8
  %103 = load ptr, ptr %2, align 8
  %104 = getelementptr inbounds i8, ptr %103, i64 6
  store ptr %104, ptr %2, align 8
  br label %10, !llvm.loop !6

105:                                              ; preds = %94, %89, %85
  %106 = load ptr, ptr %2, align 8
  %107 = call i32 @strncmp(ptr noundef %106, ptr noundef @.str.19, i64 noundef 2) #6
  %108 = icmp eq i32 %107, 0
  br i1 %108, label %109, label %125

109:                                              ; preds = %105
  %110 = load ptr, ptr %2, align 8
  %111 = getelementptr inbounds i8, ptr %110, i64 2
  %112 = load i8, ptr %111, align 1
  %113 = call zeroext i1 @is_alpha(i8 noundef signext %112)
  br i1 %113, label %125, label %114

114:                                              ; preds = %109
  %115 = load ptr, ptr %2, align 8
  %116 = getelementptr inbounds i8, ptr %115, i64 2
  %117 = load i8, ptr %116, align 1
  %118 = call zeroext i1 @is_num(i8 noundef signext %117)
  br i1 %118, label %125, label %119

119:                                              ; preds = %114
  %120 = load ptr, ptr %4, align 8
  %121 = load ptr, ptr %2, align 8
  %122 = call ptr @new_token(i32 noundef 5, ptr noundef %120, ptr noundef %121)
  store ptr %122, ptr %4, align 8
  %123 = load ptr, ptr %2, align 8
  %124 = getelementptr inbounds i8, ptr %123, i64 2
  store ptr %124, ptr %2, align 8
  br label %10, !llvm.loop !6

125:                                              ; preds = %114, %109, %105
  %126 = load ptr, ptr %2, align 8
  %127 = call i32 @strncmp(ptr noundef %126, ptr noundef @.str.20, i64 noundef 4) #6
  %128 = icmp eq i32 %127, 0
  br i1 %128, label %129, label %145

129:                                              ; preds = %125
  %130 = load ptr, ptr %2, align 8
  %131 = getelementptr inbounds i8, ptr %130, i64 4
  %132 = load i8, ptr %131, align 1
  %133 = call zeroext i1 @is_alpha(i8 noundef signext %132)
  br i1 %133, label %145, label %134

134:                                              ; preds = %129
  %135 = load ptr, ptr %2, align 8
  %136 = getelementptr inbounds i8, ptr %135, i64 4
  %137 = load i8, ptr %136, align 1
  %138 = call zeroext i1 @is_num(i8 noundef signext %137)
  br i1 %138, label %145, label %139

139:                                              ; preds = %134
  %140 = load ptr, ptr %4, align 8
  %141 = load ptr, ptr %2, align 8
  %142 = call ptr @new_token(i32 noundef 6, ptr noundef %140, ptr noundef %141)
  store ptr %142, ptr %4, align 8
  %143 = load ptr, ptr %2, align 8
  %144 = getelementptr inbounds i8, ptr %143, i64 4
  store ptr %144, ptr %2, align 8
  br label %10, !llvm.loop !6

145:                                              ; preds = %134, %129, %125
  %146 = load ptr, ptr %2, align 8
  %147 = call i32 @strncmp(ptr noundef %146, ptr noundef @.str.21, i64 noundef 5) #6
  %148 = icmp eq i32 %147, 0
  br i1 %148, label %149, label %165

149:                                              ; preds = %145
  %150 = load ptr, ptr %2, align 8
  %151 = getelementptr inbounds i8, ptr %150, i64 5
  %152 = load i8, ptr %151, align 1
  %153 = call zeroext i1 @is_alpha(i8 noundef signext %152)
  br i1 %153, label %165, label %154

154:                                              ; preds = %149
  %155 = load ptr, ptr %2, align 8
  %156 = getelementptr inbounds i8, ptr %155, i64 5
  %157 = load i8, ptr %156, align 1
  %158 = call zeroext i1 @is_num(i8 noundef signext %157)
  br i1 %158, label %165, label %159

159:                                              ; preds = %154
  %160 = load ptr, ptr %4, align 8
  %161 = load ptr, ptr %2, align 8
  %162 = call ptr @new_token(i32 noundef 7, ptr noundef %160, ptr noundef %161)
  store ptr %162, ptr %4, align 8
  %163 = load ptr, ptr %2, align 8
  %164 = getelementptr inbounds i8, ptr %163, i64 5
  store ptr %164, ptr %2, align 8
  br label %10, !llvm.loop !6

165:                                              ; preds = %154, %149, %145
  %166 = load ptr, ptr %2, align 8
  %167 = call i32 @strncmp(ptr noundef %166, ptr noundef @.str.22, i64 noundef 2) #6
  %168 = icmp eq i32 %167, 0
  br i1 %168, label %169, label %185

169:                                              ; preds = %165
  %170 = load ptr, ptr %2, align 8
  %171 = getelementptr inbounds i8, ptr %170, i64 2
  %172 = load i8, ptr %171, align 1
  %173 = call zeroext i1 @is_alpha(i8 noundef signext %172)
  br i1 %173, label %185, label %174

174:                                              ; preds = %169
  %175 = load ptr, ptr %2, align 8
  %176 = getelementptr inbounds i8, ptr %175, i64 2
  %177 = load i8, ptr %176, align 1
  %178 = call zeroext i1 @is_num(i8 noundef signext %177)
  br i1 %178, label %185, label %179

179:                                              ; preds = %174
  %180 = load ptr, ptr %4, align 8
  %181 = load ptr, ptr %2, align 8
  %182 = call ptr @new_token(i32 noundef 8, ptr noundef %180, ptr noundef %181)
  store ptr %182, ptr %4, align 8
  %183 = load ptr, ptr %2, align 8
  %184 = getelementptr inbounds i8, ptr %183, i64 2
  store ptr %184, ptr %2, align 8
  br label %10, !llvm.loop !6

185:                                              ; preds = %174, %169, %165
  %186 = load ptr, ptr %2, align 8
  %187 = call i32 @strncmp(ptr noundef %186, ptr noundef @.str.23, i64 noundef 3) #6
  %188 = icmp eq i32 %187, 0
  br i1 %188, label %189, label %205

189:                                              ; preds = %185
  %190 = load ptr, ptr %2, align 8
  %191 = getelementptr inbounds i8, ptr %190, i64 3
  %192 = load i8, ptr %191, align 1
  %193 = call zeroext i1 @is_alpha(i8 noundef signext %192)
  br i1 %193, label %205, label %194

194:                                              ; preds = %189
  %195 = load ptr, ptr %2, align 8
  %196 = getelementptr inbounds i8, ptr %195, i64 3
  %197 = load i8, ptr %196, align 1
  %198 = call zeroext i1 @is_num(i8 noundef signext %197)
  br i1 %198, label %205, label %199

199:                                              ; preds = %194
  %200 = load ptr, ptr %4, align 8
  %201 = load ptr, ptr %2, align 8
  %202 = call ptr @new_token(i32 noundef 9, ptr noundef %200, ptr noundef %201)
  store ptr %202, ptr %4, align 8
  %203 = load ptr, ptr %2, align 8
  %204 = getelementptr inbounds i8, ptr %203, i64 3
  store ptr %204, ptr %2, align 8
  br label %10, !llvm.loop !6

205:                                              ; preds = %194, %189, %185
  %206 = load ptr, ptr %2, align 8
  %207 = call i32 @strncmp(ptr noundef %206, ptr noundef @.str.24, i64 noundef 8) #6
  %208 = icmp eq i32 %207, 0
  br i1 %208, label %209, label %225

209:                                              ; preds = %205
  %210 = load ptr, ptr %2, align 8
  %211 = getelementptr inbounds i8, ptr %210, i64 8
  %212 = load i8, ptr %211, align 1
  %213 = call zeroext i1 @is_alpha(i8 noundef signext %212)
  br i1 %213, label %225, label %214

214:                                              ; preds = %209
  %215 = load ptr, ptr %2, align 8
  %216 = getelementptr inbounds i8, ptr %215, i64 8
  %217 = load i8, ptr %216, align 1
  %218 = call zeroext i1 @is_num(i8 noundef signext %217)
  br i1 %218, label %225, label %219

219:                                              ; preds = %214
  %220 = load ptr, ptr %4, align 8
  %221 = load ptr, ptr %2, align 8
  %222 = call ptr @new_token(i32 noundef 10, ptr noundef %220, ptr noundef %221)
  store ptr %222, ptr %4, align 8
  %223 = load ptr, ptr %2, align 8
  %224 = getelementptr inbounds i8, ptr %223, i64 8
  store ptr %224, ptr %2, align 8
  br label %10, !llvm.loop !6

225:                                              ; preds = %214, %209, %205
  %226 = load ptr, ptr %2, align 8
  %227 = call i32 @strncmp(ptr noundef %226, ptr noundef @.str.25, i64 noundef 5) #6
  %228 = icmp eq i32 %227, 0
  br i1 %228, label %229, label %245

229:                                              ; preds = %225
  %230 = load ptr, ptr %2, align 8
  %231 = getelementptr inbounds i8, ptr %230, i64 5
  %232 = load i8, ptr %231, align 1
  %233 = call zeroext i1 @is_alpha(i8 noundef signext %232)
  br i1 %233, label %245, label %234

234:                                              ; preds = %229
  %235 = load ptr, ptr %2, align 8
  %236 = getelementptr inbounds i8, ptr %235, i64 5
  %237 = load i8, ptr %236, align 1
  %238 = call zeroext i1 @is_num(i8 noundef signext %237)
  br i1 %238, label %245, label %239

239:                                              ; preds = %234
  %240 = load ptr, ptr %4, align 8
  %241 = load ptr, ptr %2, align 8
  %242 = call ptr @new_token(i32 noundef 11, ptr noundef %240, ptr noundef %241)
  store ptr %242, ptr %4, align 8
  %243 = load ptr, ptr %2, align 8
  %244 = getelementptr inbounds i8, ptr %243, i64 5
  store ptr %244, ptr %2, align 8
  br label %10, !llvm.loop !6

245:                                              ; preds = %234, %229, %225
  %246 = load ptr, ptr %2, align 8
  %247 = call i32 @strncmp(ptr noundef %246, ptr noundef @.str.26, i64 noundef 6) #6
  %248 = icmp eq i32 %247, 0
  br i1 %248, label %249, label %265

249:                                              ; preds = %245
  %250 = load ptr, ptr %2, align 8
  %251 = getelementptr inbounds i8, ptr %250, i64 6
  %252 = load i8, ptr %251, align 1
  %253 = call zeroext i1 @is_alpha(i8 noundef signext %252)
  br i1 %253, label %265, label %254

254:                                              ; preds = %249
  %255 = load ptr, ptr %2, align 8
  %256 = getelementptr inbounds i8, ptr %255, i64 6
  %257 = load i8, ptr %256, align 1
  %258 = call zeroext i1 @is_num(i8 noundef signext %257)
  br i1 %258, label %265, label %259

259:                                              ; preds = %254
  %260 = load ptr, ptr %4, align 8
  %261 = load ptr, ptr %2, align 8
  %262 = call ptr @new_token(i32 noundef 12, ptr noundef %260, ptr noundef %261)
  store ptr %262, ptr %4, align 8
  %263 = load ptr, ptr %2, align 8
  %264 = getelementptr inbounds i8, ptr %263, i64 6
  store ptr %264, ptr %2, align 8
  br label %10, !llvm.loop !6

265:                                              ; preds = %254, %249, %245
  %266 = load ptr, ptr %2, align 8
  %267 = call i32 @strncmp(ptr noundef %266, ptr noundef @.str.27, i64 noundef 4) #6
  %268 = icmp eq i32 %267, 0
  br i1 %268, label %269, label %285

269:                                              ; preds = %265
  %270 = load ptr, ptr %2, align 8
  %271 = getelementptr inbounds i8, ptr %270, i64 4
  %272 = load i8, ptr %271, align 1
  %273 = call zeroext i1 @is_alpha(i8 noundef signext %272)
  br i1 %273, label %285, label %274

274:                                              ; preds = %269
  %275 = load ptr, ptr %2, align 8
  %276 = getelementptr inbounds i8, ptr %275, i64 4
  %277 = load i8, ptr %276, align 1
  %278 = call zeroext i1 @is_num(i8 noundef signext %277)
  br i1 %278, label %285, label %279

279:                                              ; preds = %274
  %280 = load ptr, ptr %4, align 8
  %281 = load ptr, ptr %2, align 8
  %282 = call ptr @new_token(i32 noundef 13, ptr noundef %280, ptr noundef %281)
  store ptr %282, ptr %4, align 8
  %283 = load ptr, ptr %2, align 8
  %284 = getelementptr inbounds i8, ptr %283, i64 4
  store ptr %284, ptr %2, align 8
  br label %10, !llvm.loop !6

285:                                              ; preds = %274, %269, %265
  %286 = load ptr, ptr %2, align 8
  %287 = call i32 @strncmp(ptr noundef %286, ptr noundef @.str.28, i64 noundef 7) #6
  %288 = icmp eq i32 %287, 0
  br i1 %288, label %289, label %305

289:                                              ; preds = %285
  %290 = load ptr, ptr %2, align 8
  %291 = getelementptr inbounds i8, ptr %290, i64 7
  %292 = load i8, ptr %291, align 1
  %293 = call zeroext i1 @is_alpha(i8 noundef signext %292)
  br i1 %293, label %305, label %294

294:                                              ; preds = %289
  %295 = load ptr, ptr %2, align 8
  %296 = getelementptr inbounds i8, ptr %295, i64 7
  %297 = load i8, ptr %296, align 1
  %298 = call zeroext i1 @is_num(i8 noundef signext %297)
  br i1 %298, label %305, label %299

299:                                              ; preds = %294
  %300 = load ptr, ptr %4, align 8
  %301 = load ptr, ptr %2, align 8
  %302 = call ptr @new_token(i32 noundef 14, ptr noundef %300, ptr noundef %301)
  store ptr %302, ptr %4, align 8
  %303 = load ptr, ptr %2, align 8
  %304 = getelementptr inbounds i8, ptr %303, i64 7
  store ptr %304, ptr %2, align 8
  br label %10, !llvm.loop !6

305:                                              ; preds = %294, %289, %285
  %306 = load ptr, ptr %2, align 8
  %307 = call i32 @strncmp(ptr noundef %306, ptr noundef @.str.29, i64 noundef 3) #6
  %308 = icmp eq i32 %307, 0
  br i1 %308, label %309, label %325

309:                                              ; preds = %305
  %310 = load ptr, ptr %2, align 8
  %311 = getelementptr inbounds i8, ptr %310, i64 3
  %312 = load i8, ptr %311, align 1
  %313 = call zeroext i1 @is_alpha(i8 noundef signext %312)
  br i1 %313, label %325, label %314

314:                                              ; preds = %309
  %315 = load ptr, ptr %2, align 8
  %316 = getelementptr inbounds i8, ptr %315, i64 3
  %317 = load i8, ptr %316, align 1
  %318 = call zeroext i1 @is_num(i8 noundef signext %317)
  br i1 %318, label %325, label %319

319:                                              ; preds = %314
  %320 = load ptr, ptr %4, align 8
  %321 = load ptr, ptr %2, align 8
  %322 = call ptr @new_token(i32 noundef 16, ptr noundef %320, ptr noundef %321)
  store ptr %322, ptr %4, align 8
  %323 = load ptr, ptr %2, align 8
  %324 = getelementptr inbounds i8, ptr %323, i64 3
  store ptr %324, ptr %2, align 8
  br label %10, !llvm.loop !6

325:                                              ; preds = %314, %309, %305
  %326 = load ptr, ptr %2, align 8
  %327 = call i32 @strncmp(ptr noundef %326, ptr noundef @.str.30, i64 noundef 4) #6
  %328 = icmp eq i32 %327, 0
  br i1 %328, label %329, label %345

329:                                              ; preds = %325
  %330 = load ptr, ptr %2, align 8
  %331 = getelementptr inbounds i8, ptr %330, i64 4
  %332 = load i8, ptr %331, align 1
  %333 = call zeroext i1 @is_alpha(i8 noundef signext %332)
  br i1 %333, label %345, label %334

334:                                              ; preds = %329
  %335 = load ptr, ptr %2, align 8
  %336 = getelementptr inbounds i8, ptr %335, i64 4
  %337 = load i8, ptr %336, align 1
  %338 = call zeroext i1 @is_num(i8 noundef signext %337)
  br i1 %338, label %345, label %339

339:                                              ; preds = %334
  %340 = load ptr, ptr %4, align 8
  %341 = load ptr, ptr %2, align 8
  %342 = call ptr @new_token(i32 noundef 17, ptr noundef %340, ptr noundef %341)
  store ptr %342, ptr %4, align 8
  %343 = load ptr, ptr %2, align 8
  %344 = getelementptr inbounds i8, ptr %343, i64 4
  store ptr %344, ptr %2, align 8
  br label %10, !llvm.loop !6

345:                                              ; preds = %334, %329, %325
  %346 = load ptr, ptr %2, align 8
  %347 = call i32 @strncmp(ptr noundef %346, ptr noundef @.str.31, i64 noundef 4) #6
  %348 = icmp eq i32 %347, 0
  br i1 %348, label %349, label %365

349:                                              ; preds = %345
  %350 = load ptr, ptr %2, align 8
  %351 = getelementptr inbounds i8, ptr %350, i64 4
  %352 = load i8, ptr %351, align 1
  %353 = call zeroext i1 @is_alpha(i8 noundef signext %352)
  br i1 %353, label %365, label %354

354:                                              ; preds = %349
  %355 = load ptr, ptr %2, align 8
  %356 = getelementptr inbounds i8, ptr %355, i64 4
  %357 = load i8, ptr %356, align 1
  %358 = call zeroext i1 @is_num(i8 noundef signext %357)
  br i1 %358, label %365, label %359

359:                                              ; preds = %354
  %360 = load ptr, ptr %4, align 8
  %361 = load ptr, ptr %2, align 8
  %362 = call ptr @new_token(i32 noundef 18, ptr noundef %360, ptr noundef %361)
  store ptr %362, ptr %4, align 8
  %363 = load ptr, ptr %2, align 8
  %364 = getelementptr inbounds i8, ptr %363, i64 4
  store ptr %364, ptr %2, align 8
  br label %10, !llvm.loop !6

365:                                              ; preds = %354, %349, %345
  %366 = load ptr, ptr %2, align 8
  %367 = call i32 @strncmp(ptr noundef %366, ptr noundef @.str.32, i64 noundef 6) #6
  %368 = icmp eq i32 %367, 0
  br i1 %368, label %369, label %385

369:                                              ; preds = %365
  %370 = load ptr, ptr %2, align 8
  %371 = getelementptr inbounds i8, ptr %370, i64 6
  %372 = load i8, ptr %371, align 1
  %373 = call zeroext i1 @is_alpha(i8 noundef signext %372)
  br i1 %373, label %385, label %374

374:                                              ; preds = %369
  %375 = load ptr, ptr %2, align 8
  %376 = getelementptr inbounds i8, ptr %375, i64 6
  %377 = load i8, ptr %376, align 1
  %378 = call zeroext i1 @is_num(i8 noundef signext %377)
  br i1 %378, label %385, label %379

379:                                              ; preds = %374
  %380 = load ptr, ptr %4, align 8
  %381 = load ptr, ptr %2, align 8
  %382 = call ptr @new_token(i32 noundef 19, ptr noundef %380, ptr noundef %381)
  store ptr %382, ptr %4, align 8
  %383 = load ptr, ptr %2, align 8
  %384 = getelementptr inbounds i8, ptr %383, i64 6
  store ptr %384, ptr %2, align 8
  br label %10, !llvm.loop !6

385:                                              ; preds = %374, %369, %365
  %386 = load ptr, ptr %2, align 8
  %387 = call i32 @strncmp(ptr noundef %386, ptr noundef @.str.33, i64 noundef 6) #6
  %388 = icmp eq i32 %387, 0
  br i1 %388, label %389, label %405

389:                                              ; preds = %385
  %390 = load ptr, ptr %2, align 8
  %391 = getelementptr inbounds i8, ptr %390, i64 6
  %392 = load i8, ptr %391, align 1
  %393 = call zeroext i1 @is_alpha(i8 noundef signext %392)
  br i1 %393, label %405, label %394

394:                                              ; preds = %389
  %395 = load ptr, ptr %2, align 8
  %396 = getelementptr inbounds i8, ptr %395, i64 6
  %397 = load i8, ptr %396, align 1
  %398 = call zeroext i1 @is_num(i8 noundef signext %397)
  br i1 %398, label %405, label %399

399:                                              ; preds = %394
  %400 = load ptr, ptr %4, align 8
  %401 = load ptr, ptr %2, align 8
  %402 = call ptr @new_token(i32 noundef 15, ptr noundef %400, ptr noundef %401)
  store ptr %402, ptr %4, align 8
  %403 = load ptr, ptr %2, align 8
  %404 = getelementptr inbounds i8, ptr %403, i64 6
  store ptr %404, ptr %2, align 8
  br label %10, !llvm.loop !6

405:                                              ; preds = %394, %389, %385
  %406 = load ptr, ptr %2, align 8
  %407 = call i32 @strncmp(ptr noundef %406, ptr noundef @.str.34, i64 noundef 4) #6
  %408 = icmp eq i32 %407, 0
  br i1 %408, label %409, label %425

409:                                              ; preds = %405
  %410 = load ptr, ptr %2, align 8
  %411 = getelementptr inbounds i8, ptr %410, i64 4
  %412 = load i8, ptr %411, align 1
  %413 = call zeroext i1 @is_alpha(i8 noundef signext %412)
  br i1 %413, label %425, label %414

414:                                              ; preds = %409
  %415 = load ptr, ptr %2, align 8
  %416 = getelementptr inbounds i8, ptr %415, i64 4
  %417 = load i8, ptr %416, align 1
  %418 = call zeroext i1 @is_num(i8 noundef signext %417)
  br i1 %418, label %425, label %419

419:                                              ; preds = %414
  %420 = load ptr, ptr %4, align 8
  %421 = load ptr, ptr %2, align 8
  %422 = call ptr @new_token(i32 noundef 20, ptr noundef %420, ptr noundef %421)
  store ptr %422, ptr %4, align 8
  %423 = load ptr, ptr %2, align 8
  %424 = getelementptr inbounds i8, ptr %423, i64 4
  store ptr %424, ptr %2, align 8
  br label %10, !llvm.loop !6

425:                                              ; preds = %414, %409, %405
  %426 = load ptr, ptr %2, align 8
  %427 = call i32 @strncmp(ptr noundef %426, ptr noundef @.str.35, i64 noundef 7) #6
  %428 = icmp eq i32 %427, 0
  br i1 %428, label %429, label %445

429:                                              ; preds = %425
  %430 = load ptr, ptr %2, align 8
  %431 = getelementptr inbounds i8, ptr %430, i64 7
  %432 = load i8, ptr %431, align 1
  %433 = call zeroext i1 @is_alpha(i8 noundef signext %432)
  br i1 %433, label %445, label %434

434:                                              ; preds = %429
  %435 = load ptr, ptr %2, align 8
  %436 = getelementptr inbounds i8, ptr %435, i64 7
  %437 = load i8, ptr %436, align 1
  %438 = call zeroext i1 @is_num(i8 noundef signext %437)
  br i1 %438, label %445, label %439

439:                                              ; preds = %434
  %440 = load ptr, ptr %4, align 8
  %441 = load ptr, ptr %2, align 8
  %442 = call ptr @new_token(i32 noundef 21, ptr noundef %440, ptr noundef %441)
  store ptr %442, ptr %4, align 8
  %443 = load ptr, ptr %2, align 8
  %444 = getelementptr inbounds i8, ptr %443, i64 7
  store ptr %444, ptr %2, align 8
  br label %10, !llvm.loop !6

445:                                              ; preds = %434, %429, %425
  %446 = load ptr, ptr %2, align 8
  %447 = call i32 @strncmp(ptr noundef %446, ptr noundef @.str.36, i64 noundef 6) #6
  %448 = icmp eq i32 %447, 0
  br i1 %448, label %449, label %465

449:                                              ; preds = %445
  %450 = load ptr, ptr %2, align 8
  %451 = getelementptr inbounds i8, ptr %450, i64 6
  %452 = load i8, ptr %451, align 1
  %453 = call zeroext i1 @is_alpha(i8 noundef signext %452)
  br i1 %453, label %465, label %454

454:                                              ; preds = %449
  %455 = load ptr, ptr %2, align 8
  %456 = getelementptr inbounds i8, ptr %455, i64 6
  %457 = load i8, ptr %456, align 1
  %458 = call zeroext i1 @is_num(i8 noundef signext %457)
  br i1 %458, label %465, label %459

459:                                              ; preds = %454
  %460 = load ptr, ptr %4, align 8
  %461 = load ptr, ptr %2, align 8
  %462 = call ptr @new_token(i32 noundef 22, ptr noundef %460, ptr noundef %461)
  store ptr %462, ptr %4, align 8
  %463 = load ptr, ptr %2, align 8
  %464 = getelementptr inbounds i8, ptr %463, i64 6
  store ptr %464, ptr %2, align 8
  br label %10, !llvm.loop !6

465:                                              ; preds = %454, %449, %445
  %466 = load ptr, ptr %2, align 8
  %467 = load i8, ptr %466, align 1
  %468 = call zeroext i1 @is_alpha(i8 noundef signext %467)
  br i1 %468, label %469, label %496

469:                                              ; preds = %465
  %470 = load ptr, ptr %2, align 8
  store ptr %470, ptr %6, align 8
  br label %471

471:                                              ; preds = %482, %469
  %472 = load ptr, ptr %2, align 8
  %473 = getelementptr inbounds i8, ptr %472, i32 1
  store ptr %473, ptr %2, align 8
  br label %474

474:                                              ; preds = %471
  %475 = load ptr, ptr %2, align 8
  %476 = load i8, ptr %475, align 1
  %477 = call zeroext i1 @is_alpha(i8 noundef signext %476)
  br i1 %477, label %482, label %478

478:                                              ; preds = %474
  %479 = load ptr, ptr %2, align 8
  %480 = load i8, ptr %479, align 1
  %481 = call zeroext i1 @is_num(i8 noundef signext %480)
  br label %482

482:                                              ; preds = %478, %474
  %483 = phi i1 [ true, %474 ], [ %481, %478 ]
  br i1 %483, label %471, label %484, !llvm.loop !9

484:                                              ; preds = %482
  %485 = load ptr, ptr %4, align 8
  %486 = load ptr, ptr %6, align 8
  %487 = call ptr @new_token(i32 noundef 1, ptr noundef %485, ptr noundef %486)
  store ptr %487, ptr %4, align 8
  %488 = load ptr, ptr %2, align 8
  %489 = load ptr, ptr %6, align 8
  %490 = ptrtoint ptr %488 to i64
  %491 = ptrtoint ptr %489 to i64
  %492 = sub i64 %490, %491
  %493 = trunc i64 %492 to i32
  %494 = load ptr, ptr %4, align 8
  %495 = getelementptr inbounds %struct.Token, ptr %494, i32 0, i32 4
  store i32 %493, ptr %495, align 8
  br label %10, !llvm.loop !6

496:                                              ; preds = %465
  %497 = load ptr, ptr %2, align 8
  %498 = load i8, ptr %497, align 1
  %499 = call zeroext i1 @is_num(i8 noundef signext %498)
  br i1 %499, label %500, label %509

500:                                              ; preds = %496
  %501 = load ptr, ptr %4, align 8
  %502 = load ptr, ptr %2, align 8
  %503 = call ptr @new_token(i32 noundef 2, ptr noundef %501, ptr noundef %502)
  store ptr %503, ptr %4, align 8
  %504 = load ptr, ptr %2, align 8
  %505 = call i64 @strtol(ptr noundef %504, ptr noundef %2, i32 noundef 10) #7
  %506 = trunc i64 %505 to i32
  %507 = load ptr, ptr %4, align 8
  %508 = getelementptr inbounds %struct.Token, ptr %507, i32 0, i32 2
  store i32 %506, ptr %508, align 8
  br label %10, !llvm.loop !6

509:                                              ; preds = %496
  %510 = load ptr, ptr %2, align 8
  %511 = load i8, ptr %510, align 1
  %512 = sext i8 %511 to i32
  %513 = icmp eq i32 %512, 39
  br i1 %513, label %514, label %555

514:                                              ; preds = %509
  %515 = load ptr, ptr %2, align 8
  %516 = getelementptr inbounds i8, ptr %515, i32 1
  store ptr %516, ptr %2, align 8
  store ptr %516, ptr %7, align 8
  %517 = load ptr, ptr %2, align 8
  %518 = load i8, ptr %517, align 1
  %519 = sext i8 %518 to i32
  %520 = icmp eq i32 %519, 92
  br i1 %520, label %521, label %524

521:                                              ; preds = %514
  %522 = load ptr, ptr %2, align 8
  %523 = getelementptr inbounds i8, ptr %522, i32 1
  store ptr %523, ptr %2, align 8
  br label %532

524:                                              ; preds = %514
  %525 = load ptr, ptr %2, align 8
  %526 = load i8, ptr %525, align 1
  %527 = sext i8 %526 to i32
  %528 = icmp eq i32 %527, 39
  br i1 %528, label %529, label %531

529:                                              ; preds = %524
  %530 = load ptr, ptr %7, align 8
  call void (ptr, ptr, ...) @error_at(ptr noundef %530, ptr noundef @.str.37)
  br label %531

531:                                              ; preds = %529, %524
  br label %532

532:                                              ; preds = %531, %521
  %533 = load ptr, ptr %2, align 8
  %534 = getelementptr inbounds i8, ptr %533, i32 1
  store ptr %534, ptr %2, align 8
  %535 = load ptr, ptr %2, align 8
  %536 = load i8, ptr %535, align 1
  %537 = sext i8 %536 to i32
  %538 = icmp ne i32 %537, 39
  br i1 %538, label %539, label %541

539:                                              ; preds = %532
  %540 = load ptr, ptr %7, align 8
  call void (ptr, ptr, ...) @error_at(ptr noundef %540, ptr noundef @.str.38)
  br label %541

541:                                              ; preds = %539, %532
  %542 = load ptr, ptr %4, align 8
  %543 = load ptr, ptr %7, align 8
  %544 = call ptr @new_token(i32 noundef 3, ptr noundef %542, ptr noundef %543)
  store ptr %544, ptr %4, align 8
  %545 = load ptr, ptr %2, align 8
  %546 = load ptr, ptr %7, align 8
  %547 = ptrtoint ptr %545 to i64
  %548 = ptrtoint ptr %546 to i64
  %549 = sub i64 %547, %548
  %550 = trunc i64 %549 to i32
  %551 = load ptr, ptr %4, align 8
  %552 = getelementptr inbounds %struct.Token, ptr %551, i32 0, i32 2
  store i32 %550, ptr %552, align 8
  %553 = load ptr, ptr %2, align 8
  %554 = getelementptr inbounds i8, ptr %553, i32 1
  store ptr %554, ptr %2, align 8
  br label %10, !llvm.loop !6

555:                                              ; preds = %509
  %556 = load ptr, ptr %2, align 8
  %557 = load i8, ptr %556, align 1
  %558 = sext i8 %557 to i32
  %559 = icmp eq i32 %558, 34
  br i1 %559, label %560, label %600

560:                                              ; preds = %555
  %561 = load ptr, ptr %2, align 8
  %562 = getelementptr inbounds i8, ptr %561, i32 1
  store ptr %562, ptr %2, align 8
  store ptr %562, ptr %8, align 8
  br label %563

563:                                              ; preds = %583, %560
  %564 = load ptr, ptr %2, align 8
  %565 = load i8, ptr %564, align 1
  %566 = sext i8 %565 to i32
  %567 = icmp ne i32 %566, 34
  br i1 %567, label %574, label %568

568:                                              ; preds = %563
  %569 = load ptr, ptr %2, align 8
  %570 = getelementptr inbounds i8, ptr %569, i64 -1
  %571 = load i8, ptr %570, align 1
  %572 = sext i8 %571 to i32
  %573 = icmp eq i32 %572, 92
  br label %574

574:                                              ; preds = %568, %563
  %575 = phi i1 [ true, %563 ], [ %573, %568 ]
  br i1 %575, label %576, label %586

576:                                              ; preds = %574
  %577 = load ptr, ptr %2, align 8
  %578 = load i8, ptr %577, align 1
  %579 = sext i8 %578 to i32
  %580 = icmp eq i32 %579, 0
  br i1 %580, label %581, label %583

581:                                              ; preds = %576
  %582 = load ptr, ptr %8, align 8
  call void (ptr, ptr, ...) @error_at(ptr noundef %582, ptr noundef @.str.39)
  br label %583

583:                                              ; preds = %581, %576
  %584 = load ptr, ptr %2, align 8
  %585 = getelementptr inbounds i8, ptr %584, i32 1
  store ptr %585, ptr %2, align 8
  br label %563, !llvm.loop !10

586:                                              ; preds = %574
  %587 = load ptr, ptr %4, align 8
  %588 = load ptr, ptr %8, align 8
  %589 = call ptr @new_token(i32 noundef 23, ptr noundef %587, ptr noundef %588)
  store ptr %589, ptr %4, align 8
  %590 = load ptr, ptr %2, align 8
  %591 = load ptr, ptr %8, align 8
  %592 = ptrtoint ptr %590 to i64
  %593 = ptrtoint ptr %591 to i64
  %594 = sub i64 %592, %593
  %595 = trunc i64 %594 to i32
  %596 = load ptr, ptr %4, align 8
  %597 = getelementptr inbounds %struct.Token, ptr %596, i32 0, i32 4
  store i32 %595, ptr %597, align 8
  %598 = load ptr, ptr %2, align 8
  %599 = getelementptr inbounds i8, ptr %598, i32 1
  store ptr %599, ptr %2, align 8
  br label %10, !llvm.loop !6

600:                                              ; preds = %555
  %601 = load ptr, ptr %2, align 8
  call void (ptr, ptr, ...) @error_at(ptr noundef %601, ptr noundef @.str.40)
  br label %10, !llvm.loop !6

602:                                              ; preds = %10
  %603 = load ptr, ptr %4, align 8
  %604 = load ptr, ptr %2, align 8
  %605 = call ptr @new_token(i32 noundef 24, ptr noundef %603, ptr noundef %604)
  %606 = getelementptr inbounds %struct.Token, ptr %3, i32 0, i32 1
  %607 = load ptr, ptr %606, align 8
  ret ptr %607
}

; Function Attrs: nounwind willreturn memory(read)
declare ptr @strstr(ptr noundef, ptr noundef) #2

declare void @error_at(ptr noundef, ptr noundef, ...) #3

; Function Attrs: nounwind
declare i64 @strtol(ptr noundef, ptr noundef, i32 noundef) #4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i1 @consume_reserved(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca i1, align 1
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  store ptr %0, ptr %4, align 8
  store i32 %1, ptr %5, align 4
  %6 = load ptr, ptr %4, align 8
  %7 = load ptr, ptr %6, align 8
  %8 = getelementptr inbounds %struct.Token, ptr %7, i32 0, i32 0
  %9 = load i32, ptr %8, align 8
  %10 = load i32, ptr %5, align 4
  %11 = icmp ne i32 %9, %10
  br i1 %11, label %12, label %13

12:                                               ; preds = %2
  store i1 false, ptr %3, align 1
  br label %19

13:                                               ; preds = %2
  %14 = load ptr, ptr %4, align 8
  %15 = load ptr, ptr %14, align 8
  %16 = getelementptr inbounds %struct.Token, ptr %15, i32 0, i32 1
  %17 = load ptr, ptr %16, align 8
  %18 = load ptr, ptr %4, align 8
  store ptr %17, ptr %18, align 8
  store i1 true, ptr %3, align 1
  br label %19

19:                                               ; preds = %13, %12
  %20 = load i1, ptr %3, align 1
  ret i1 %20
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @expect_reserved(ptr noundef %0, i32 noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  store i32 %1, ptr %4, align 4
  %5 = load ptr, ptr %3, align 8
  %6 = load ptr, ptr %5, align 8
  %7 = getelementptr inbounds %struct.Token, ptr %6, i32 0, i32 0
  %8 = load i32, ptr %7, align 8
  %9 = load i32, ptr %4, align 4
  %10 = icmp ne i32 %8, %9
  br i1 %10, label %11, label %16

11:                                               ; preds = %2
  %12 = load ptr, ptr %3, align 8
  %13 = load ptr, ptr %12, align 8
  %14 = getelementptr inbounds %struct.Token, ptr %13, i32 0, i32 3
  %15 = load ptr, ptr %14, align 8
  call void (ptr, ptr, ...) @error_at(ptr noundef %15, ptr noundef @.str.41)
  br label %16

16:                                               ; preds = %11, %2
  %17 = load ptr, ptr %3, align 8
  %18 = load ptr, ptr %17, align 8
  %19 = getelementptr inbounds %struct.Token, ptr %18, i32 0, i32 1
  %20 = load ptr, ptr %19, align 8
  %21 = load ptr, ptr %3, align 8
  store ptr %20, ptr %21, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local zeroext i1 @consume(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca i1, align 1
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  %6 = load ptr, ptr %4, align 8
  %7 = load ptr, ptr %6, align 8
  %8 = getelementptr inbounds %struct.Token, ptr %7, i32 0, i32 0
  %9 = load i32, ptr %8, align 8
  %10 = icmp ne i32 %9, 0
  br i1 %10, label %33, label %11

11:                                               ; preds = %2
  %12 = load ptr, ptr %5, align 8
  %13 = call i64 @strlen(ptr noundef %12) #6
  %14 = trunc i64 %13 to i32
  %15 = load ptr, ptr %4, align 8
  %16 = load ptr, ptr %15, align 8
  %17 = getelementptr inbounds %struct.Token, ptr %16, i32 0, i32 4
  %18 = load i32, ptr %17, align 8
  %19 = icmp ne i32 %14, %18
  br i1 %19, label %33, label %20

20:                                               ; preds = %11
  %21 = load ptr, ptr %4, align 8
  %22 = load ptr, ptr %21, align 8
  %23 = getelementptr inbounds %struct.Token, ptr %22, i32 0, i32 3
  %24 = load ptr, ptr %23, align 8
  %25 = load ptr, ptr %5, align 8
  %26 = load ptr, ptr %4, align 8
  %27 = load ptr, ptr %26, align 8
  %28 = getelementptr inbounds %struct.Token, ptr %27, i32 0, i32 4
  %29 = load i32, ptr %28, align 8
  %30 = sext i32 %29 to i64
  %31 = call i32 @memcmp(ptr noundef %24, ptr noundef %25, i64 noundef %30) #6
  %32 = icmp ne i32 %31, 0
  br i1 %32, label %33, label %34

33:                                               ; preds = %20, %11, %2
  store i1 false, ptr %3, align 1
  br label %40

34:                                               ; preds = %20
  %35 = load ptr, ptr %4, align 8
  %36 = load ptr, ptr %35, align 8
  %37 = getelementptr inbounds %struct.Token, ptr %36, i32 0, i32 1
  %38 = load ptr, ptr %37, align 8
  %39 = load ptr, ptr %4, align 8
  store ptr %38, ptr %39, align 8
  store i1 true, ptr %3, align 1
  br label %40

40:                                               ; preds = %34, %33
  %41 = load i1, ptr %3, align 1
  ret i1 %41
}

; Function Attrs: nounwind willreturn memory(read)
declare i64 @strlen(ptr noundef) #2

; Function Attrs: nounwind willreturn memory(read)
declare i32 @memcmp(ptr noundef, ptr noundef, i64 noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @consume_number(ptr noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  %4 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  %5 = load ptr, ptr %3, align 8
  %6 = load ptr, ptr %5, align 8
  %7 = getelementptr inbounds %struct.Token, ptr %6, i32 0, i32 0
  %8 = load i32, ptr %7, align 8
  %9 = icmp ne i32 %8, 2
  br i1 %9, label %10, label %11

10:                                               ; preds = %1
  store i32 -1, ptr %2, align 4
  br label %22

11:                                               ; preds = %1
  %12 = load ptr, ptr %3, align 8
  %13 = load ptr, ptr %12, align 8
  %14 = getelementptr inbounds %struct.Token, ptr %13, i32 0, i32 2
  %15 = load i32, ptr %14, align 8
  store i32 %15, ptr %4, align 4
  %16 = load ptr, ptr %3, align 8
  %17 = load ptr, ptr %16, align 8
  %18 = getelementptr inbounds %struct.Token, ptr %17, i32 0, i32 1
  %19 = load ptr, ptr %18, align 8
  %20 = load ptr, ptr %3, align 8
  store ptr %19, ptr %20, align 8
  %21 = load i32, ptr %4, align 4
  store i32 %21, ptr %2, align 4
  br label %22

22:                                               ; preds = %11, %10
  %23 = load i32, ptr %2, align 4
  ret i32 %23
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @consume_ident(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  %5 = load ptr, ptr %3, align 8
  %6 = load ptr, ptr %5, align 8
  %7 = getelementptr inbounds %struct.Token, ptr %6, i32 0, i32 0
  %8 = load i32, ptr %7, align 8
  %9 = icmp ne i32 %8, 1
  br i1 %9, label %10, label %11

10:                                               ; preds = %1
  store ptr null, ptr %2, align 8
  br label %20

11:                                               ; preds = %1
  %12 = load ptr, ptr %3, align 8
  %13 = load ptr, ptr %12, align 8
  store ptr %13, ptr %4, align 8
  %14 = load ptr, ptr %3, align 8
  %15 = load ptr, ptr %14, align 8
  %16 = getelementptr inbounds %struct.Token, ptr %15, i32 0, i32 1
  %17 = load ptr, ptr %16, align 8
  %18 = load ptr, ptr %3, align 8
  store ptr %17, ptr %18, align 8
  %19 = load ptr, ptr %4, align 8
  store ptr %19, ptr %2, align 8
  br label %20

20:                                               ; preds = %11, %10
  %21 = load ptr, ptr %2, align 8
  ret ptr %21
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @expect(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store ptr %1, ptr %4, align 8
  %5 = load ptr, ptr %3, align 8
  %6 = load ptr, ptr %5, align 8
  %7 = getelementptr inbounds %struct.Token, ptr %6, i32 0, i32 0
  %8 = load i32, ptr %7, align 8
  %9 = icmp ne i32 %8, 0
  br i1 %9, label %32, label %10

10:                                               ; preds = %2
  %11 = load ptr, ptr %4, align 8
  %12 = call i64 @strlen(ptr noundef %11) #6
  %13 = trunc i64 %12 to i32
  %14 = load ptr, ptr %3, align 8
  %15 = load ptr, ptr %14, align 8
  %16 = getelementptr inbounds %struct.Token, ptr %15, i32 0, i32 4
  %17 = load i32, ptr %16, align 8
  %18 = icmp ne i32 %13, %17
  br i1 %18, label %32, label %19

19:                                               ; preds = %10
  %20 = load ptr, ptr %3, align 8
  %21 = load ptr, ptr %20, align 8
  %22 = getelementptr inbounds %struct.Token, ptr %21, i32 0, i32 3
  %23 = load ptr, ptr %22, align 8
  %24 = load ptr, ptr %4, align 8
  %25 = load ptr, ptr %3, align 8
  %26 = load ptr, ptr %25, align 8
  %27 = getelementptr inbounds %struct.Token, ptr %26, i32 0, i32 4
  %28 = load i32, ptr %27, align 8
  %29 = sext i32 %28 to i64
  %30 = call i32 @memcmp(ptr noundef %23, ptr noundef %24, i64 noundef %29) #6
  %31 = icmp ne i32 %30, 0
  br i1 %31, label %32, label %38

32:                                               ; preds = %19, %10, %2
  %33 = load ptr, ptr %3, align 8
  %34 = load ptr, ptr %33, align 8
  %35 = getelementptr inbounds %struct.Token, ptr %34, i32 0, i32 3
  %36 = load ptr, ptr %35, align 8
  %37 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @error_at(ptr noundef %36, ptr noundef @.str.42, ptr noundef %37)
  br label %38

38:                                               ; preds = %32, %19
  %39 = load ptr, ptr %3, align 8
  %40 = load ptr, ptr %39, align 8
  %41 = getelementptr inbounds %struct.Token, ptr %40, i32 0, i32 1
  %42 = load ptr, ptr %41, align 8
  %43 = load ptr, ptr %3, align 8
  store ptr %42, ptr %43, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @expect_number(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca i32, align 4
  store ptr %0, ptr %2, align 8
  %4 = load ptr, ptr %2, align 8
  %5 = load ptr, ptr %4, align 8
  %6 = getelementptr inbounds %struct.Token, ptr %5, i32 0, i32 0
  %7 = load i32, ptr %6, align 8
  %8 = icmp ne i32 %7, 2
  br i1 %8, label %9, label %14

9:                                                ; preds = %1
  %10 = load ptr, ptr %2, align 8
  %11 = load ptr, ptr %10, align 8
  %12 = getelementptr inbounds %struct.Token, ptr %11, i32 0, i32 3
  %13 = load ptr, ptr %12, align 8
  call void (ptr, ptr, ...) @error_at(ptr noundef %13, ptr noundef @.str.43)
  br label %14

14:                                               ; preds = %9, %1
  %15 = load ptr, ptr %2, align 8
  %16 = load ptr, ptr %15, align 8
  %17 = getelementptr inbounds %struct.Token, ptr %16, i32 0, i32 2
  %18 = load i32, ptr %17, align 8
  store i32 %18, ptr %3, align 4
  %19 = load ptr, ptr %2, align 8
  %20 = load ptr, ptr %19, align 8
  %21 = getelementptr inbounds %struct.Token, ptr %20, i32 0, i32 1
  %22 = load ptr, ptr %21, align 8
  %23 = load ptr, ptr %2, align 8
  store ptr %22, ptr %23, align 8
  %24 = load i32, ptr %3, align 4
  ret i32 %24
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind allocsize(0,1) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind willreturn memory(read) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind allocsize(0,1) }
attributes #6 = { nounwind willreturn memory(read) }
attributes #7 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 18.1.3 (1)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
