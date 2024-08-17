; ModuleID = 'src/preprocessor.c'
source_filename = "src/preprocessor.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.vector = type { ptr, i32, i32 }

@.str = private unnamed_addr constant [2 x i8] c".\00", align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@.str.2 = private unnamed_addr constant [6 x i8] c"%s/%s\00", align 1
@.str.3 = private unnamed_addr constant [52 x i8] c"\E4\B8\8D\E6\AD\A3\E3\81\AA include \E3\83\87\E3\82\A3\E3\83\AC\E3\82\AF\E3\83\86\E3\82\A3\E3\83\96\E3\81\A7\E3\81\99: %.*s\00", align 1
@.str.4 = private unnamed_addr constant [61 x i8] c"\E9\96\89\E3\81\98\E3\82\89\E3\82\8C\E3\81\A6\E3\81\84\E3\81\AA\E3\81\84 include \E3\83\87\E3\82\A3\E3\83\AC\E3\82\AF\E3\83\86\E3\82\A3\E3\83\96\E3\81\A7\E3\81\99\00", align 1
@filename = external global ptr, align 8
@.str.5 = private unnamed_addr constant [19 x i8] c"File not found: %s\00", align 1
@.str.6 = private unnamed_addr constant [51 x i8] c"\E4\B8\8D\E6\AD\A3\E3\81\AA define \E3\83\87\E3\82\A3\E3\83\AC\E3\82\AF\E3\83\86\E3\82\A3\E3\83\96\E3\81\A7\E3\81\99: %.*s\00", align 1
@.str.7 = private unnamed_addr constant [48 x i8] c"\1B[34m[preprocessor] \1B[32m[#define] \1B[0mlen = %d\00", align 1
@.str.8 = private unnamed_addr constant [49 x i8] c"\1B[34m[preprocessor] \1B[32m[#define] \1B[0mname = %s\00", align 1
@.str.9 = private unnamed_addr constant [58 x i8] c"\E5\BC\95\E6\95\B0\E4\BB\98\E3\81\8D\E3\83\9E\E3\82\AF\E3\83\AD\E3\81\AF\E3\82\B5\E3\83\9D\E3\83\BC\E3\83\88\E3\81\95\E3\82\8C\E3\81\A6\E3\81\84\E3\81\BE\E3\81\9B\E3\82\93\00", align 1
@.str.10 = private unnamed_addr constant [49 x i8] c"\1B[34m[preprocessor] \1B[32m[#define] \1B[0mbody = %s\00", align 1
@.str.11 = private unnamed_addr constant [3 x i8] c"//\00", align 1
@.str.12 = private unnamed_addr constant [3 x i8] c"/*\00", align 1
@.str.13 = private unnamed_addr constant [3 x i8] c"*/\00", align 1
@.str.14 = private unnamed_addr constant [63 x i8] c"\1B[34m[preprocessor] \1B[32m[#define] \1B[0mstrlen(new_output) = %d\00", align 1
@.str.15 = private unnamed_addr constant [150 x i8] c"#define NULL ((void*)0)\0A#define true 1\0A#define false 0\0A#define SEEK_END 2\0A#define SEEK_SET 0\0A#define SEEK_CUR 1\0A#define errno (*__errno_location ())\0A\00", align 1
@.str.16 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@.str.17 = private unnamed_addr constant [47 x i8] c"\1B[34m[preprocessor] \1B[0mStart preprocessing...\00", align 1
@.str.18 = private unnamed_addr constant [10 x i8] c"#include \00", align 1
@.str.19 = private unnamed_addr constant [49 x i8] c"\1B[34m[preprocessor] \1B[0mFound #include directive\00", align 1
@.str.20 = private unnamed_addr constant [9 x i8] c"#pragma \00", align 1
@.str.21 = private unnamed_addr constant [48 x i8] c"\1B[34m[preprocessor] \1B[0mFound #pragma directive\00", align 1
@.str.22 = private unnamed_addr constant [9 x i8] c"#define \00", align 1
@.str.23 = private unnamed_addr constant [48 x i8] c"\1B[34m[preprocessor] \1B[0mFound #define directive\00", align 1
@.str.24 = private unnamed_addr constant [43 x i8] c"\1B[34m[preprocessor] \1B[0mPreprocessing done\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @dirname(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  %6 = load ptr, ptr %3, align 8
  %7 = load ptr, ptr %3, align 8
  %8 = call i64 @strlen(ptr noundef %7) #6
  %9 = getelementptr inbounds i8, ptr %6, i64 %8
  %10 = getelementptr inbounds i8, ptr %9, i64 -1
  store ptr %10, ptr %4, align 8
  br label %11

11:                                               ; preds = %22, %1
  %12 = load ptr, ptr %4, align 8
  %13 = load ptr, ptr %3, align 8
  %14 = icmp uge ptr %12, %13
  br i1 %14, label %15, label %20

15:                                               ; preds = %11
  %16 = load ptr, ptr %4, align 8
  %17 = load i8, ptr %16, align 1
  %18 = sext i8 %17 to i32
  %19 = icmp ne i32 %18, 47
  br label %20

20:                                               ; preds = %15, %11
  %21 = phi i1 [ false, %11 ], [ %19, %15 ]
  br i1 %21, label %22, label %25

22:                                               ; preds = %20
  %23 = load ptr, ptr %4, align 8
  %24 = getelementptr inbounds i8, ptr %23, i32 -1
  store ptr %24, ptr %4, align 8
  br label %11, !llvm.loop !6

25:                                               ; preds = %20
  %26 = load ptr, ptr %4, align 8
  %27 = load ptr, ptr %3, align 8
  %28 = icmp ult ptr %26, %27
  br i1 %28, label %29, label %30

29:                                               ; preds = %25
  store ptr @.str, ptr %2, align 8
  br label %48

30:                                               ; preds = %25
  %31 = load ptr, ptr %4, align 8
  %32 = load ptr, ptr %3, align 8
  %33 = ptrtoint ptr %31 to i64
  %34 = ptrtoint ptr %32 to i64
  %35 = sub i64 %33, %34
  %36 = add nsw i64 %35, 1
  %37 = call noalias ptr @malloc(i64 noundef %36) #7
  store ptr %37, ptr %5, align 8
  %38 = load ptr, ptr %5, align 8
  %39 = load ptr, ptr %4, align 8
  %40 = load ptr, ptr %3, align 8
  %41 = ptrtoint ptr %39 to i64
  %42 = ptrtoint ptr %40 to i64
  %43 = sub i64 %41, %42
  %44 = add nsw i64 %43, 1
  %45 = load ptr, ptr %3, align 8
  %46 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef %38, i64 noundef %44, ptr noundef @.str.1, ptr noundef %45) #8
  %47 = load ptr, ptr %5, align 8
  store ptr %47, ptr %2, align 8
  br label %48

48:                                               ; preds = %30, %29
  %49 = load ptr, ptr %2, align 8
  ret ptr %49
}

; Function Attrs: nounwind willreturn memory(read)
declare i64 @strlen(ptr noundef) #1

; Function Attrs: nounwind allocsize(0)
declare noalias ptr @malloc(i64 noundef) #2

; Function Attrs: nounwind
declare i32 @snprintf(ptr noundef, i64 noundef, ptr noundef, ...) #3

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @join_path(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  store ptr %1, ptr %4, align 8
  %6 = load ptr, ptr %3, align 8
  %7 = call i64 @strlen(ptr noundef %6) #6
  %8 = load ptr, ptr %4, align 8
  %9 = call i64 @strlen(ptr noundef %8) #6
  %10 = add i64 %7, %9
  %11 = add i64 %10, 2
  %12 = call noalias ptr @malloc(i64 noundef %11) #7
  store ptr %12, ptr %5, align 8
  %13 = load ptr, ptr %5, align 8
  %14 = load ptr, ptr %3, align 8
  %15 = call i64 @strlen(ptr noundef %14) #6
  %16 = load ptr, ptr %4, align 8
  %17 = call i64 @strlen(ptr noundef %16) #6
  %18 = add i64 %15, %17
  %19 = add i64 %18, 2
  %20 = load ptr, ptr %3, align 8
  %21 = load ptr, ptr %4, align 8
  %22 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef %13, i64 noundef %19, ptr noundef @.str.2, ptr noundef %20, ptr noundef %21) #8
  %23 = load ptr, ptr %5, align 8
  ret ptr %23
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @process_include(ptr noundef %0, ptr noundef %1) #0 {
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca i8, align 1
  %8 = alloca ptr, align 8
  %9 = alloca ptr, align 8
  %10 = alloca i32, align 4
  %11 = alloca ptr, align 8
  %12 = alloca ptr, align 8
  %13 = alloca ptr, align 8
  %14 = alloca ptr, align 8
  %15 = alloca ptr, align 8
  %16 = alloca ptr, align 8
  store ptr %0, ptr %4, align 8
  store ptr %1, ptr %5, align 8
  %17 = load ptr, ptr %4, align 8
  %18 = getelementptr inbounds i8, ptr %17, i64 9
  store ptr %18, ptr %6, align 8
  br label %19

19:                                               ; preds = %24, %2
  %20 = load ptr, ptr %6, align 8
  %21 = load i8, ptr %20, align 1
  %22 = sext i8 %21 to i32
  %23 = icmp eq i32 %22, 32
  br i1 %23, label %24, label %27

24:                                               ; preds = %19
  %25 = load ptr, ptr %6, align 8
  %26 = getelementptr inbounds i8, ptr %25, i32 1
  store ptr %26, ptr %6, align 8
  br label %19, !llvm.loop !8

27:                                               ; preds = %19
  %28 = load ptr, ptr %6, align 8
  %29 = load i8, ptr %28, align 1
  %30 = sext i8 %29 to i32
  %31 = icmp ne i32 %30, 60
  br i1 %31, label %32, label %40

32:                                               ; preds = %27
  %33 = load ptr, ptr %6, align 8
  %34 = load i8, ptr %33, align 1
  %35 = sext i8 %34 to i32
  %36 = icmp ne i32 %35, 34
  br i1 %36, label %37, label %40

37:                                               ; preds = %32
  %38 = load ptr, ptr %6, align 8
  %39 = load ptr, ptr %6, align 8
  call void (ptr, ptr, ...) @error_at(ptr noundef %38, ptr noundef @.str.3, i32 noundef 1, ptr noundef %39)
  store ptr null, ptr %3, align 8
  br label %151

40:                                               ; preds = %32, %27
  %41 = load ptr, ptr %6, align 8
  %42 = load i8, ptr %41, align 1
  %43 = sext i8 %42 to i32
  %44 = icmp eq i32 %43, 60
  %45 = zext i1 %44 to i8
  store i8 %45, ptr %7, align 1
  %46 = load ptr, ptr %6, align 8
  %47 = getelementptr inbounds i8, ptr %46, i32 1
  store ptr %47, ptr %6, align 8
  %48 = load ptr, ptr %6, align 8
  store ptr %48, ptr %8, align 8
  br label %49

49:                                               ; preds = %66, %40
  %50 = load ptr, ptr %8, align 8
  %51 = load i8, ptr %50, align 1
  %52 = sext i8 %51 to i32
  %53 = icmp ne i32 %52, 0
  br i1 %53, label %54, label %64

54:                                               ; preds = %49
  %55 = load ptr, ptr %8, align 8
  %56 = load i8, ptr %55, align 1
  %57 = sext i8 %56 to i32
  %58 = icmp ne i32 %57, 62
  br i1 %58, label %59, label %64

59:                                               ; preds = %54
  %60 = load ptr, ptr %8, align 8
  %61 = load i8, ptr %60, align 1
  %62 = sext i8 %61 to i32
  %63 = icmp ne i32 %62, 34
  br label %64

64:                                               ; preds = %59, %54, %49
  %65 = phi i1 [ false, %54 ], [ false, %49 ], [ %63, %59 ]
  br i1 %65, label %66, label %69

66:                                               ; preds = %64
  %67 = load ptr, ptr %8, align 8
  %68 = getelementptr inbounds i8, ptr %67, i32 1
  store ptr %68, ptr %8, align 8
  br label %49, !llvm.loop !9

69:                                               ; preds = %64
  %70 = load ptr, ptr %8, align 8
  %71 = load i8, ptr %70, align 1
  %72 = sext i8 %71 to i32
  %73 = icmp eq i32 %72, 0
  br i1 %73, label %74, label %76

74:                                               ; preds = %69
  %75 = load ptr, ptr %8, align 8
  call void (ptr, ptr, ...) @error_at(ptr noundef %75, ptr noundef @.str.4)
  store ptr null, ptr %3, align 8
  br label %151

76:                                               ; preds = %69
  %77 = load ptr, ptr %8, align 8
  %78 = load ptr, ptr %6, align 8
  %79 = ptrtoint ptr %77 to i64
  %80 = ptrtoint ptr %78 to i64
  %81 = sub i64 %79, %80
  %82 = add nsw i64 %81, 1
  %83 = call noalias ptr @malloc(i64 noundef %82) #7
  store ptr %83, ptr %9, align 8
  %84 = load ptr, ptr %9, align 8
  %85 = load ptr, ptr %8, align 8
  %86 = load ptr, ptr %6, align 8
  %87 = ptrtoint ptr %85 to i64
  %88 = ptrtoint ptr %86 to i64
  %89 = sub i64 %87, %88
  %90 = add nsw i64 %89, 1
  %91 = load ptr, ptr %6, align 8
  %92 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef %84, i64 noundef %90, ptr noundef @.str.1, ptr noundef %91) #8
  %93 = load i8, ptr %7, align 1
  %94 = trunc i8 %93 to i1
  br i1 %94, label %95, label %98

95:                                               ; preds = %76
  %96 = load ptr, ptr %8, align 8
  %97 = getelementptr inbounds i8, ptr %96, i64 1
  store ptr %97, ptr %3, align 8
  br label %151

98:                                               ; preds = %76
  store i32 0, ptr %10, align 4
  br label %99

99:                                               ; preds = %117, %98
  %100 = load i32, ptr %10, align 4
  %101 = load ptr, ptr %5, align 8
  %102 = getelementptr inbounds %struct.vector, ptr %101, i32 0, i32 1
  %103 = load i32, ptr %102, align 8
  %104 = icmp slt i32 %100, %103
  br i1 %104, label %105, label %120

105:                                              ; preds = %99
  %106 = load ptr, ptr %5, align 8
  %107 = load i32, ptr %10, align 4
  %108 = call ptr @vec_at(ptr noundef %106, i32 noundef %107)
  store ptr %108, ptr %11, align 8
  %109 = load ptr, ptr %11, align 8
  %110 = load ptr, ptr %9, align 8
  %111 = call i32 @strcmp(ptr noundef %109, ptr noundef %110) #6
  %112 = icmp eq i32 %111, 0
  br i1 %112, label %113, label %116

113:                                              ; preds = %105
  %114 = load ptr, ptr %8, align 8
  %115 = getelementptr inbounds i8, ptr %114, i64 1
  store ptr %115, ptr %3, align 8
  br label %151

116:                                              ; preds = %105
  br label %117

117:                                              ; preds = %116
  %118 = load i32, ptr %10, align 4
  %119 = add nsw i32 %118, 1
  store i32 %119, ptr %10, align 4
  br label %99, !llvm.loop !10

120:                                              ; preds = %99
  %121 = load ptr, ptr %5, align 8
  %122 = load ptr, ptr %9, align 8
  call void @vec_push_last(ptr noundef %121, ptr noundef %122)
  %123 = load ptr, ptr @filename, align 8
  %124 = call ptr @dirname(ptr noundef %123)
  store ptr %124, ptr %12, align 8
  %125 = load ptr, ptr %12, align 8
  %126 = load ptr, ptr %9, align 8
  %127 = call ptr @join_path(ptr noundef %125, ptr noundef %126)
  store ptr %127, ptr %13, align 8
  %128 = load ptr, ptr %13, align 8
  %129 = call ptr @read_file(ptr noundef %128)
  store ptr %129, ptr %14, align 8
  %130 = load ptr, ptr %14, align 8
  %131 = icmp ne ptr %130, null
  br i1 %131, label %134, label %132

132:                                              ; preds = %120
  %133 = load ptr, ptr %9, align 8
  call void (ptr, ...) @error(ptr noundef @.str.5, ptr noundef %133)
  store ptr null, ptr %3, align 8
  br label %151

134:                                              ; preds = %120
  %135 = load ptr, ptr %8, align 8
  %136 = getelementptr inbounds i8, ptr %135, i64 1
  store ptr %136, ptr %15, align 8
  %137 = load ptr, ptr %14, align 8
  %138 = call i64 @strlen(ptr noundef %137) #6
  %139 = load ptr, ptr %15, align 8
  %140 = call i64 @strlen(ptr noundef %139) #6
  %141 = add i64 %138, %140
  %142 = add i64 %141, 1
  %143 = call noalias ptr @calloc(i64 noundef %142, i64 noundef 1) #9
  store ptr %143, ptr %16, align 8
  %144 = load ptr, ptr %16, align 8
  %145 = load ptr, ptr %14, align 8
  %146 = call ptr @strcat(ptr noundef %144, ptr noundef %145) #8
  %147 = load ptr, ptr %16, align 8
  %148 = load ptr, ptr %15, align 8
  %149 = call ptr @strcat(ptr noundef %147, ptr noundef %148) #8
  %150 = load ptr, ptr %16, align 8
  store ptr %150, ptr %3, align 8
  br label %151

151:                                              ; preds = %134, %132, %113, %95, %74, %37
  %152 = load ptr, ptr %3, align 8
  ret ptr %152
}

declare void @error_at(ptr noundef, ptr noundef, ...) #4

declare ptr @vec_at(ptr noundef, i32 noundef) #4

; Function Attrs: nounwind willreturn memory(read)
declare i32 @strcmp(ptr noundef, ptr noundef) #1

declare void @vec_push_last(ptr noundef, ptr noundef) #4

declare ptr @read_file(ptr noundef) #4

declare void @error(ptr noundef, ...) #4

; Function Attrs: nounwind allocsize(0,1)
declare noalias ptr @calloc(i64 noundef, i64 noundef) #5

; Function Attrs: nounwind
declare ptr @strcat(ptr noundef, ptr noundef) #3

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @process_define(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca ptr, align 8
  %8 = alloca ptr, align 8
  %9 = alloca ptr, align 8
  %10 = alloca ptr, align 8
  %11 = alloca ptr, align 8
  %12 = alloca ptr, align 8
  store ptr %0, ptr %3, align 8
  %13 = load ptr, ptr %3, align 8
  %14 = getelementptr inbounds i8, ptr %13, i64 8
  store ptr %14, ptr %4, align 8
  br label %15

15:                                               ; preds = %20, %1
  %16 = load ptr, ptr %4, align 8
  %17 = load i8, ptr %16, align 1
  %18 = sext i8 %17 to i32
  %19 = icmp eq i32 %18, 32
  br i1 %19, label %20, label %23

20:                                               ; preds = %15
  %21 = load ptr, ptr %4, align 8
  %22 = getelementptr inbounds i8, ptr %21, i32 1
  store ptr %22, ptr %4, align 8
  br label %15, !llvm.loop !11

23:                                               ; preds = %15
  %24 = load ptr, ptr %4, align 8
  %25 = load i8, ptr %24, align 1
  %26 = call zeroext i1 @is_alpha(i8 noundef signext %25)
  br i1 %26, label %30, label %27

27:                                               ; preds = %23
  %28 = load ptr, ptr %4, align 8
  %29 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @error_at(ptr noundef %28, ptr noundef @.str.6, i32 noundef 1, ptr noundef %29)
  store ptr null, ptr %2, align 8
  br label %284

30:                                               ; preds = %23
  %31 = load ptr, ptr %4, align 8
  %32 = getelementptr inbounds i8, ptr %31, i64 1
  store ptr %32, ptr %5, align 8
  br label %33

33:                                               ; preds = %43, %30
  %34 = load ptr, ptr %5, align 8
  %35 = load i8, ptr %34, align 1
  %36 = call zeroext i1 @is_alpha(i8 noundef signext %35)
  br i1 %36, label %41, label %37

37:                                               ; preds = %33
  %38 = load ptr, ptr %5, align 8
  %39 = load i8, ptr %38, align 1
  %40 = call zeroext i1 @is_num(i8 noundef signext %39)
  br label %41

41:                                               ; preds = %37, %33
  %42 = phi i1 [ true, %33 ], [ %40, %37 ]
  br i1 %42, label %43, label %46

43:                                               ; preds = %41
  %44 = load ptr, ptr %5, align 8
  %45 = getelementptr inbounds i8, ptr %44, i32 1
  store ptr %45, ptr %5, align 8
  br label %33, !llvm.loop !12

46:                                               ; preds = %41
  %47 = load ptr, ptr %5, align 8
  %48 = load ptr, ptr %4, align 8
  %49 = ptrtoint ptr %47 to i64
  %50 = ptrtoint ptr %48 to i64
  %51 = sub i64 %49, %50
  %52 = trunc i64 %51 to i32
  store i32 %52, ptr %6, align 4
  %53 = load i32, ptr %6, align 4
  call void (ptr, ...) @print_debug(ptr noundef @.str.7, i32 noundef %53)
  %54 = load i32, ptr %6, align 4
  %55 = add nsw i32 %54, 1
  %56 = sext i32 %55 to i64
  %57 = call noalias ptr @malloc(i64 noundef %56) #7
  store ptr %57, ptr %7, align 8
  %58 = load ptr, ptr %7, align 8
  %59 = load i32, ptr %6, align 4
  %60 = add nsw i32 %59, 1
  %61 = sext i32 %60 to i64
  %62 = load ptr, ptr %4, align 8
  %63 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef %58, i64 noundef %61, ptr noundef @.str.1, ptr noundef %62) #8
  %64 = load ptr, ptr %7, align 8
  call void (ptr, ...) @print_debug(ptr noundef @.str.8, ptr noundef %64)
  %65 = load ptr, ptr %5, align 8
  %66 = load i8, ptr %65, align 1
  %67 = sext i8 %66 to i32
  %68 = icmp eq i32 %67, 40
  br i1 %68, label %69, label %71

69:                                               ; preds = %46
  %70 = load ptr, ptr %5, align 8
  call void (ptr, ptr, ...) @error_at(ptr noundef %70, ptr noundef @.str.9)
  store ptr null, ptr %2, align 8
  br label %284

71:                                               ; preds = %46
  br label %72

72:                                               ; preds = %77, %71
  %73 = load ptr, ptr %5, align 8
  %74 = load i8, ptr %73, align 1
  %75 = sext i8 %74 to i32
  %76 = icmp eq i32 %75, 32
  br i1 %76, label %77, label %80

77:                                               ; preds = %72
  %78 = load ptr, ptr %5, align 8
  %79 = getelementptr inbounds i8, ptr %78, i32 1
  store ptr %79, ptr %5, align 8
  br label %72, !llvm.loop !13

80:                                               ; preds = %72
  %81 = load ptr, ptr %5, align 8
  %82 = load i8, ptr %81, align 1
  %83 = sext i8 %82 to i32
  %84 = icmp eq i32 %83, 10
  br i1 %84, label %85, label %88

85:                                               ; preds = %80
  %86 = load ptr, ptr %7, align 8
  call void @free(ptr noundef %86) #8
  %87 = load ptr, ptr %5, align 8
  store ptr %87, ptr %2, align 8
  br label %284

88:                                               ; preds = %80
  %89 = load ptr, ptr %5, align 8
  store ptr %89, ptr %8, align 8
  br label %90

90:                                               ; preds = %95, %88
  %91 = load ptr, ptr %8, align 8
  %92 = load i8, ptr %91, align 1
  %93 = sext i8 %92 to i32
  %94 = icmp ne i32 %93, 10
  br i1 %94, label %95, label %98

95:                                               ; preds = %90
  %96 = load ptr, ptr %8, align 8
  %97 = getelementptr inbounds i8, ptr %96, i32 1
  store ptr %97, ptr %8, align 8
  br label %90, !llvm.loop !14

98:                                               ; preds = %90
  %99 = load ptr, ptr %8, align 8
  %100 = load ptr, ptr %5, align 8
  %101 = ptrtoint ptr %99 to i64
  %102 = ptrtoint ptr %100 to i64
  %103 = sub i64 %101, %102
  %104 = add nsw i64 %103, 1
  %105 = call noalias ptr @malloc(i64 noundef %104) #7
  store ptr %105, ptr %9, align 8
  %106 = load ptr, ptr %9, align 8
  %107 = load ptr, ptr %8, align 8
  %108 = load ptr, ptr %5, align 8
  %109 = ptrtoint ptr %107 to i64
  %110 = ptrtoint ptr %108 to i64
  %111 = sub i64 %109, %110
  %112 = add nsw i64 %111, 1
  %113 = load ptr, ptr %5, align 8
  %114 = call i32 (ptr, i64, ptr, ...) @snprintf(ptr noundef %106, i64 noundef %112, ptr noundef @.str.1, ptr noundef %113) #8
  %115 = load ptr, ptr %9, align 8
  call void (ptr, ...) @print_debug(ptr noundef @.str.10, ptr noundef %115)
  %116 = load ptr, ptr %8, align 8
  %117 = getelementptr inbounds i8, ptr %116, i32 1
  store ptr %117, ptr %8, align 8
  store ptr %116, ptr %10, align 8
  %118 = call noalias ptr @calloc(i64 noundef 1048576, i64 noundef 1) #9
  store ptr %118, ptr %11, align 8
  br label %119

119:                                              ; preds = %268, %98
  %120 = load ptr, ptr %8, align 8
  %121 = load i8, ptr %120, align 1
  %122 = sext i8 %121 to i32
  %123 = icmp ne i32 %122, 0
  br i1 %123, label %124, label %129

124:                                              ; preds = %119
  %125 = load ptr, ptr %8, align 8
  %126 = load i8, ptr %125, align 1
  %127 = sext i8 %126 to i32
  %128 = icmp ne i32 %127, 0
  br label %129

129:                                              ; preds = %124, %119
  %130 = phi i1 [ false, %119 ], [ %128, %124 ]
  br i1 %130, label %131, label %271

131:                                              ; preds = %129
  %132 = load ptr, ptr %8, align 8
  %133 = call i32 @strncmp(ptr noundef %132, ptr noundef @.str.11, i64 noundef 2) #6
  %134 = icmp eq i32 %133, 0
  br i1 %134, label %135, label %151

135:                                              ; preds = %131
  br label %136

136:                                              ; preds = %147, %135
  %137 = load ptr, ptr %8, align 8
  %138 = load i8, ptr %137, align 1
  %139 = sext i8 %138 to i32
  %140 = icmp ne i32 %139, 10
  br i1 %140, label %141, label %150

141:                                              ; preds = %136
  %142 = load ptr, ptr %8, align 8
  %143 = load i8, ptr %142, align 1
  %144 = sext i8 %143 to i32
  %145 = icmp eq i32 %144, 0
  br i1 %145, label %146, label %147

146:                                              ; preds = %141
  br label %150

147:                                              ; preds = %141
  %148 = load ptr, ptr %8, align 8
  %149 = getelementptr inbounds i8, ptr %148, i32 1
  store ptr %149, ptr %8, align 8
  br label %136, !llvm.loop !15

150:                                              ; preds = %146, %136
  br label %151

151:                                              ; preds = %150, %131
  %152 = load ptr, ptr %8, align 8
  %153 = call i32 @strncmp(ptr noundef %152, ptr noundef @.str.12, i64 noundef 2) #6
  %154 = icmp eq i32 %153, 0
  br i1 %154, label %155, label %170

155:                                              ; preds = %151
  br label %156

156:                                              ; preds = %166, %155
  %157 = load ptr, ptr %8, align 8
  %158 = call i32 @strncmp(ptr noundef %157, ptr noundef @.str.13, i64 noundef 2) #6
  %159 = icmp ne i32 %158, 0
  br i1 %159, label %160, label %169

160:                                              ; preds = %156
  %161 = load ptr, ptr %8, align 8
  %162 = load i8, ptr %161, align 1
  %163 = sext i8 %162 to i32
  %164 = icmp eq i32 %163, 0
  br i1 %164, label %165, label %166

165:                                              ; preds = %160
  br label %169

166:                                              ; preds = %160
  %167 = load ptr, ptr %8, align 8
  %168 = getelementptr inbounds i8, ptr %167, i32 1
  store ptr %168, ptr %8, align 8
  br label %156, !llvm.loop !16

169:                                              ; preds = %165, %156
  br label %170

170:                                              ; preds = %169, %151
  %171 = load ptr, ptr %8, align 8
  %172 = load i8, ptr %171, align 1
  %173 = sext i8 %172 to i32
  %174 = icmp eq i32 %173, 34
  br i1 %174, label %175, label %206

175:                                              ; preds = %170
  %176 = load ptr, ptr %8, align 8
  %177 = getelementptr inbounds i8, ptr %176, i32 1
  store ptr %177, ptr %8, align 8
  br label %178

178:                                              ; preds = %202, %175
  %179 = load ptr, ptr %8, align 8
  %180 = load i8, ptr %179, align 1
  %181 = sext i8 %180 to i32
  %182 = icmp ne i32 %181, 34
  br i1 %182, label %189, label %183

183:                                              ; preds = %178
  %184 = load ptr, ptr %8, align 8
  %185 = getelementptr inbounds i8, ptr %184, i64 -1
  %186 = load i8, ptr %185, align 1
  %187 = sext i8 %186 to i32
  %188 = icmp eq i32 %187, 92
  br i1 %188, label %189, label %194

189:                                              ; preds = %183, %178
  %190 = load ptr, ptr %8, align 8
  %191 = load i8, ptr %190, align 1
  %192 = sext i8 %191 to i32
  %193 = icmp ne i32 %192, 10
  br label %194

194:                                              ; preds = %189, %183
  %195 = phi i1 [ false, %183 ], [ %193, %189 ]
  br i1 %195, label %196, label %205

196:                                              ; preds = %194
  %197 = load ptr, ptr %8, align 8
  %198 = load i8, ptr %197, align 1
  %199 = sext i8 %198 to i32
  %200 = icmp eq i32 %199, 0
  br i1 %200, label %201, label %202

201:                                              ; preds = %196
  br label %205

202:                                              ; preds = %196
  %203 = load ptr, ptr %8, align 8
  %204 = getelementptr inbounds i8, ptr %203, i32 1
  store ptr %204, ptr %8, align 8
  br label %178, !llvm.loop !17

205:                                              ; preds = %201, %194
  br label %206

206:                                              ; preds = %205, %170
  %207 = load ptr, ptr %8, align 8
  %208 = load i8, ptr %207, align 1
  %209 = sext i8 %208 to i32
  %210 = icmp eq i32 %209, 39
  br i1 %210, label %211, label %242

211:                                              ; preds = %206
  %212 = load ptr, ptr %8, align 8
  %213 = getelementptr inbounds i8, ptr %212, i32 1
  store ptr %213, ptr %8, align 8
  br label %214

214:                                              ; preds = %238, %211
  %215 = load ptr, ptr %8, align 8
  %216 = load i8, ptr %215, align 1
  %217 = sext i8 %216 to i32
  %218 = icmp ne i32 %217, 39
  br i1 %218, label %225, label %219

219:                                              ; preds = %214
  %220 = load ptr, ptr %8, align 8
  %221 = getelementptr inbounds i8, ptr %220, i64 -1
  %222 = load i8, ptr %221, align 1
  %223 = sext i8 %222 to i32
  %224 = icmp eq i32 %223, 92
  br i1 %224, label %225, label %230

225:                                              ; preds = %219, %214
  %226 = load ptr, ptr %8, align 8
  %227 = load i8, ptr %226, align 1
  %228 = sext i8 %227 to i32
  %229 = icmp ne i32 %228, 10
  br label %230

230:                                              ; preds = %225, %219
  %231 = phi i1 [ false, %219 ], [ %229, %225 ]
  br i1 %231, label %232, label %241

232:                                              ; preds = %230
  %233 = load ptr, ptr %8, align 8
  %234 = load i8, ptr %233, align 1
  %235 = sext i8 %234 to i32
  %236 = icmp eq i32 %235, 0
  br i1 %236, label %237, label %238

237:                                              ; preds = %232
  br label %241

238:                                              ; preds = %232
  %239 = load ptr, ptr %8, align 8
  %240 = getelementptr inbounds i8, ptr %239, i32 1
  store ptr %240, ptr %8, align 8
  br label %214, !llvm.loop !18

241:                                              ; preds = %237, %230
  br label %242

242:                                              ; preds = %241, %206
  %243 = load ptr, ptr %8, align 8
  %244 = load ptr, ptr %7, align 8
  %245 = load i32, ptr %6, align 4
  %246 = sext i32 %245 to i64
  %247 = call i32 @strncmp(ptr noundef %243, ptr noundef %244, i64 noundef %246) #6
  %248 = icmp eq i32 %247, 0
  br i1 %248, label %249, label %268

249:                                              ; preds = %242
  %250 = load ptr, ptr %11, align 8
  %251 = load ptr, ptr %10, align 8
  %252 = load ptr, ptr %8, align 8
  %253 = load ptr, ptr %10, align 8
  %254 = ptrtoint ptr %252 to i64
  %255 = ptrtoint ptr %253 to i64
  %256 = sub i64 %254, %255
  %257 = call ptr @strncat(ptr noundef %250, ptr noundef %251, i64 noundef %256) #8
  %258 = load ptr, ptr %11, align 8
  %259 = load ptr, ptr %9, align 8
  %260 = call ptr @strcat(ptr noundef %258, ptr noundef %259) #8
  %261 = load ptr, ptr %11, align 8
  %262 = call i64 @strlen(ptr noundef %261) #6
  call void (ptr, ...) @print_debug(ptr noundef @.str.14, i64 noundef %262)
  %263 = load i32, ptr %6, align 4
  %264 = load ptr, ptr %8, align 8
  %265 = sext i32 %263 to i64
  %266 = getelementptr inbounds i8, ptr %264, i64 %265
  store ptr %266, ptr %8, align 8
  %267 = load ptr, ptr %8, align 8
  store ptr %267, ptr %10, align 8
  br label %268

268:                                              ; preds = %249, %242
  %269 = load ptr, ptr %8, align 8
  %270 = getelementptr inbounds i8, ptr %269, i32 1
  store ptr %270, ptr %8, align 8
  br label %119, !llvm.loop !19

271:                                              ; preds = %129
  %272 = load ptr, ptr %11, align 8
  %273 = load ptr, ptr %10, align 8
  %274 = call ptr @strcat(ptr noundef %272, ptr noundef %273) #8
  %275 = load ptr, ptr %11, align 8
  %276 = call i64 @strlen(ptr noundef %275) #6
  %277 = add i64 %276, 1
  %278 = call noalias ptr @calloc(i64 noundef %277, i64 noundef 1) #9
  store ptr %278, ptr %12, align 8
  %279 = load ptr, ptr %12, align 8
  %280 = load ptr, ptr %11, align 8
  %281 = call ptr @strcat(ptr noundef %279, ptr noundef %280) #8
  %282 = load ptr, ptr %11, align 8
  call void @free(ptr noundef %282) #8
  %283 = load ptr, ptr %12, align 8
  store ptr %283, ptr %2, align 8
  br label %284

284:                                              ; preds = %271, %85, %69, %27
  %285 = load ptr, ptr %2, align 8
  ret ptr %285
}

declare zeroext i1 @is_alpha(i8 noundef signext) #4

declare zeroext i1 @is_num(i8 noundef signext) #4

declare void @print_debug(ptr noundef, ...) #4

; Function Attrs: nounwind
declare void @free(ptr noundef) #3

; Function Attrs: nounwind willreturn memory(read)
declare i32 @strncmp(ptr noundef, ptr noundef, i64 noundef) #1

; Function Attrs: nounwind
declare ptr @strncat(ptr noundef, ptr noundef, i64 noundef) #3

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @preprocess(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca ptr, align 8
  %8 = alloca ptr, align 8
  %9 = alloca ptr, align 8
  store ptr %0, ptr %2, align 8
  store ptr @.str.15, ptr %3, align 8
  %10 = load ptr, ptr %2, align 8
  %11 = call i64 @strlen(ptr noundef %10) #6
  %12 = load ptr, ptr %3, align 8
  %13 = call i64 @strlen(ptr noundef %12) #6
  %14 = add i64 %11, %13
  %15 = add i64 %14, 1
  %16 = call noalias ptr @malloc(i64 noundef %15) #7
  store ptr %16, ptr %4, align 8
  %17 = load ptr, ptr %4, align 8
  %18 = load ptr, ptr %3, align 8
  %19 = load ptr, ptr %2, align 8
  %20 = call i32 (ptr, ptr, ...) @sprintf(ptr noundef %17, ptr noundef @.str.16, ptr noundef %18, ptr noundef %19) #8
  %21 = call noalias ptr @malloc(i64 noundef 1048576) #7
  store ptr %21, ptr %5, align 8
  %22 = call ptr @new_vector()
  store ptr %22, ptr %6, align 8
  call void (ptr, ...) @print_debug(ptr noundef @.str.17)
  %23 = load ptr, ptr %4, align 8
  store ptr %23, ptr %7, align 8
  %24 = load ptr, ptr %7, align 8
  store ptr %24, ptr %8, align 8
  br label %25

25:                                               ; preds = %94, %1
  %26 = load ptr, ptr %7, align 8
  %27 = load i8, ptr %26, align 1
  %28 = icmp ne i8 %27, 0
  br i1 %28, label %29, label %97

29:                                               ; preds = %25
  %30 = load ptr, ptr %7, align 8
  %31 = call i32 @strncmp(ptr noundef %30, ptr noundef @.str.18, i64 noundef 9) #6
  %32 = icmp eq i32 %31, 0
  br i1 %32, label %33, label %46

33:                                               ; preds = %29
  call void (ptr, ...) @print_debug(ptr noundef @.str.19)
  %34 = load ptr, ptr %5, align 8
  %35 = load ptr, ptr %8, align 8
  %36 = load ptr, ptr %7, align 8
  %37 = load ptr, ptr %8, align 8
  %38 = ptrtoint ptr %36 to i64
  %39 = ptrtoint ptr %37 to i64
  %40 = sub i64 %38, %39
  %41 = call ptr @strncat(ptr noundef %34, ptr noundef %35, i64 noundef %40) #8
  %42 = load ptr, ptr %7, align 8
  %43 = load ptr, ptr %6, align 8
  %44 = call ptr @process_include(ptr noundef %42, ptr noundef %43)
  store ptr %44, ptr %7, align 8
  %45 = load ptr, ptr %7, align 8
  store ptr %45, ptr %8, align 8
  br label %46

46:                                               ; preds = %33, %29
  %47 = load ptr, ptr %7, align 8
  %48 = call i32 @strncmp(ptr noundef %47, ptr noundef @.str.20, i64 noundef 8) #6
  %49 = icmp eq i32 %48, 0
  br i1 %49, label %50, label %69

50:                                               ; preds = %46
  call void (ptr, ...) @print_debug(ptr noundef @.str.21)
  %51 = load ptr, ptr %5, align 8
  %52 = load ptr, ptr %8, align 8
  %53 = load ptr, ptr %7, align 8
  %54 = load ptr, ptr %8, align 8
  %55 = ptrtoint ptr %53 to i64
  %56 = ptrtoint ptr %54 to i64
  %57 = sub i64 %55, %56
  %58 = call ptr @strncat(ptr noundef %51, ptr noundef %52, i64 noundef %57) #8
  br label %59

59:                                               ; preds = %64, %50
  %60 = load ptr, ptr %7, align 8
  %61 = load i8, ptr %60, align 1
  %62 = sext i8 %61 to i32
  %63 = icmp ne i32 %62, 10
  br i1 %63, label %64, label %67

64:                                               ; preds = %59
  %65 = load ptr, ptr %7, align 8
  %66 = getelementptr inbounds i8, ptr %65, i32 1
  store ptr %66, ptr %7, align 8
  br label %59, !llvm.loop !20

67:                                               ; preds = %59
  %68 = load ptr, ptr %7, align 8
  store ptr %68, ptr %8, align 8
  br label %69

69:                                               ; preds = %67, %46
  %70 = load ptr, ptr %7, align 8
  %71 = call i32 @strncmp(ptr noundef %70, ptr noundef @.str.22, i64 noundef 8) #6
  %72 = icmp eq i32 %71, 0
  br i1 %72, label %73, label %85

73:                                               ; preds = %69
  call void (ptr, ...) @print_debug(ptr noundef @.str.23)
  %74 = load ptr, ptr %5, align 8
  %75 = load ptr, ptr %8, align 8
  %76 = load ptr, ptr %7, align 8
  %77 = load ptr, ptr %8, align 8
  %78 = ptrtoint ptr %76 to i64
  %79 = ptrtoint ptr %77 to i64
  %80 = sub i64 %78, %79
  %81 = call ptr @strncat(ptr noundef %74, ptr noundef %75, i64 noundef %80) #8
  %82 = load ptr, ptr %7, align 8
  %83 = call ptr @process_define(ptr noundef %82)
  store ptr %83, ptr %7, align 8
  %84 = load ptr, ptr %7, align 8
  store ptr %84, ptr %8, align 8
  br label %85

85:                                               ; preds = %73, %69
  br label %86

86:                                               ; preds = %91, %85
  %87 = load ptr, ptr %7, align 8
  %88 = load i8, ptr %87, align 1
  %89 = sext i8 %88 to i32
  %90 = icmp ne i32 %89, 10
  br i1 %90, label %91, label %94

91:                                               ; preds = %86
  %92 = load ptr, ptr %7, align 8
  %93 = getelementptr inbounds i8, ptr %92, i32 1
  store ptr %93, ptr %7, align 8
  br label %86, !llvm.loop !21

94:                                               ; preds = %86
  %95 = load ptr, ptr %7, align 8
  %96 = getelementptr inbounds i8, ptr %95, i32 1
  store ptr %96, ptr %7, align 8
  br label %25, !llvm.loop !22

97:                                               ; preds = %25
  %98 = load ptr, ptr %5, align 8
  %99 = load ptr, ptr %8, align 8
  %100 = load ptr, ptr %7, align 8
  %101 = load ptr, ptr %8, align 8
  %102 = ptrtoint ptr %100 to i64
  %103 = ptrtoint ptr %101 to i64
  %104 = sub i64 %102, %103
  %105 = call ptr @strncat(ptr noundef %98, ptr noundef %99, i64 noundef %104) #8
  call void (ptr, ...) @print_debug(ptr noundef @.str.24)
  %106 = load ptr, ptr %5, align 8
  %107 = call i64 @strlen(ptr noundef %106) #6
  %108 = add i64 %107, 1
  %109 = call noalias ptr @calloc(i64 noundef %108, i64 noundef 1) #9
  store ptr %109, ptr %9, align 8
  %110 = load ptr, ptr %9, align 8
  %111 = load ptr, ptr %5, align 8
  %112 = call ptr @strcat(ptr noundef %110, ptr noundef %111) #8
  %113 = load ptr, ptr %5, align 8
  call void @free(ptr noundef %113) #8
  %114 = load ptr, ptr %9, align 8
  ret ptr %114
}

; Function Attrs: nounwind
declare i32 @sprintf(ptr noundef, ptr noundef, ...) #3

declare ptr @new_vector() #4

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind willreturn memory(read) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind allocsize(0,1) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { nounwind willreturn memory(read) }
attributes #7 = { nounwind allocsize(0) }
attributes #8 = { nounwind }
attributes #9 = { nounwind allocsize(0,1) }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 18.1.3 (1ubuntu1)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
!13 = distinct !{!13, !7}
!14 = distinct !{!14, !7}
!15 = distinct !{!15, !7}
!16 = distinct !{!16, !7}
!17 = distinct !{!17, !7}
!18 = distinct !{!18, !7}
!19 = distinct !{!19, !7}
!20 = distinct !{!20, !7}
!21 = distinct !{!21, !7}
!22 = distinct !{!22, !7}
