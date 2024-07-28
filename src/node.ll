; ModuleID = 'src/node.c'
source_filename = "src/node.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.Node = type { ptr, ptr, ptr, ptr, ptr, ptr, ptr, ptr, i32, i32, i32 }
%struct.vector = type { ptr, i32, i32 }
%struct.Call = type { ptr, i32, ptr, ptr }
%struct.Function = type { ptr, i32, i32, i8, i8, ptr, ptr, ptr, ptr }

@.str = private unnamed_addr constant [5 x i8] c"(%s)\00", align 1
@.str.1 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.2 = private unnamed_addr constant [3 x i8] c"%c\00", align 1
@.str.3 = private unnamed_addr constant [2 x i8] c"=\00", align 1
@.str.4 = private unnamed_addr constant [8 x i8] c"return \00", align 1
@.str.5 = private unnamed_addr constant [5 x i8] c"if (\00", align 1
@.str.6 = private unnamed_addr constant [3 x i8] c") \00", align 1
@.str.7 = private unnamed_addr constant [7 x i8] c" else \00", align 1
@.str.8 = private unnamed_addr constant [8 x i8] c"while (\00", align 1
@.str.9 = private unnamed_addr constant [6 x i8] c"for (\00", align 1
@.str.10 = private unnamed_addr constant [2 x i8] c";\00", align 1
@.str.11 = private unnamed_addr constant [2 x i8] c")\00", align 1
@.str.12 = private unnamed_addr constant [3 x i8] c"{ \00", align 1
@.str.13 = private unnamed_addr constant [3 x i8] c"; \00", align 1
@.str.14 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@.str.15 = private unnamed_addr constant [6 x i8] c"%.*s(\00", align 1
@.str.16 = private unnamed_addr constant [3 x i8] c", \00", align 1
@.str.17 = private unnamed_addr constant [3 x i8] c"++\00", align 1
@.str.18 = private unnamed_addr constant [3 x i8] c"--\00", align 1
@.str.19 = private unnamed_addr constant [2 x i8] c"&\00", align 1
@.str.20 = private unnamed_addr constant [2 x i8] c"*\00", align 1
@.str.21 = private unnamed_addr constant [2 x i8] c".\00", align 1
@.str.22 = private unnamed_addr constant [4 x i8] c" ? \00", align 1
@.str.23 = private unnamed_addr constant [4 x i8] c" : \00", align 1
@.str.24 = private unnamed_addr constant [10 x i8] c"continue;\00", align 1
@.str.25 = private unnamed_addr constant [7 x i8] c"break;\00", align 1
@.str.26 = private unnamed_addr constant [9 x i8] c"switch (\00", align 1
@.str.27 = private unnamed_addr constant [4 x i8] c") {\00", align 1
@.str.28 = private unnamed_addr constant [6 x i8] c"case \00", align 1
@.str.29 = private unnamed_addr constant [3 x i8] c": \00", align 1
@.str.30 = private unnamed_addr constant [10 x i8] c"default: \00", align 1
@.str.31 = private unnamed_addr constant [2 x i8] c"(\00", align 1
@.str.32 = private unnamed_addr constant [2 x i8] c"+\00", align 1
@.str.33 = private unnamed_addr constant [2 x i8] c"-\00", align 1
@.str.34 = private unnamed_addr constant [2 x i8] c"/\00", align 1
@.str.35 = private unnamed_addr constant [3 x i8] c"==\00", align 1
@.str.36 = private unnamed_addr constant [3 x i8] c"!=\00", align 1
@.str.37 = private unnamed_addr constant [2 x i8] c"<\00", align 1
@.str.38 = private unnamed_addr constant [3 x i8] c"<=\00", align 1
@.str.39 = private unnamed_addr constant [2 x i8] c">\00", align 1
@.str.40 = private unnamed_addr constant [3 x i8] c">=\00", align 1
@.str.41 = private unnamed_addr constant [3 x i8] c"&&\00", align 1
@.str.42 = private unnamed_addr constant [3 x i8] c"||\00", align 1
@.str.43 = private unnamed_addr constant [7 x i8] c"ND_ADD\00", align 1
@.str.44 = private unnamed_addr constant [7 x i8] c"ND_SUB\00", align 1
@.str.45 = private unnamed_addr constant [7 x i8] c"ND_MUL\00", align 1
@.str.46 = private unnamed_addr constant [7 x i8] c"ND_DIV\00", align 1
@.str.47 = private unnamed_addr constant [6 x i8] c"ND_EQ\00", align 1
@.str.48 = private unnamed_addr constant [6 x i8] c"ND_NE\00", align 1
@.str.49 = private unnamed_addr constant [6 x i8] c"ND_LT\00", align 1
@.str.50 = private unnamed_addr constant [6 x i8] c"ND_LE\00", align 1
@.str.51 = private unnamed_addr constant [6 x i8] c"ND_GT\00", align 1
@.str.52 = private unnamed_addr constant [6 x i8] c"ND_GE\00", align 1
@.str.53 = private unnamed_addr constant [7 x i8] c"ND_AND\00", align 1
@.str.54 = private unnamed_addr constant [6 x i8] c"ND_OR\00", align 1
@.str.55 = private unnamed_addr constant [8 x i8] c"ND_INCR\00", align 1
@.str.56 = private unnamed_addr constant [8 x i8] c"ND_DECR\00", align 1
@.str.57 = private unnamed_addr constant [7 x i8] c"ND_REF\00", align 1
@.str.58 = private unnamed_addr constant [9 x i8] c"ND_DEREF\00", align 1
@.str.59 = private unnamed_addr constant [10 x i8] c"ND_ACCESS\00", align 1
@.str.60 = private unnamed_addr constant [10 x i8] c"ND_ASSIGN\00", align 1
@.str.61 = private unnamed_addr constant [8 x i8] c"ND_LVAR\00", align 1
@.str.62 = private unnamed_addr constant [10 x i8] c"ND_RETURN\00", align 1
@.str.63 = private unnamed_addr constant [6 x i8] c"ND_IF\00", align 1
@.str.64 = private unnamed_addr constant [9 x i8] c"ND_WHILE\00", align 1
@.str.65 = private unnamed_addr constant [7 x i8] c"ND_FOR\00", align 1
@.str.66 = private unnamed_addr constant [12 x i8] c"ND_CONTINUE\00", align 1
@.str.67 = private unnamed_addr constant [9 x i8] c"ND_BREAK\00", align 1
@.str.68 = private unnamed_addr constant [10 x i8] c"ND_SWITCH\00", align 1
@.str.69 = private unnamed_addr constant [8 x i8] c"ND_CASE\00", align 1
@.str.70 = private unnamed_addr constant [11 x i8] c"ND_DEFAULT\00", align 1
@.str.71 = private unnamed_addr constant [9 x i8] c"ND_BLOCK\00", align 1
@.str.72 = private unnamed_addr constant [9 x i8] c"ND_GROUP\00", align 1
@.str.73 = private unnamed_addr constant [8 x i8] c"ND_CALL\00", align 1
@.str.74 = private unnamed_addr constant [7 x i8] c"ND_NUM\00", align 1
@.str.75 = private unnamed_addr constant [10 x i8] c"ND_STRING\00", align 1
@.str.76 = private unnamed_addr constant [11 x i8] c"ND_TERNARY\00", align 1
@.str.77 = private unnamed_addr constant [8 x i8] c"UNKNOWN\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @new_node(i32 noundef %0, ptr noundef %1, ptr noundef %2) #0 {
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca ptr, align 8
  store i32 %0, ptr %4, align 4
  store ptr %1, ptr %5, align 8
  store ptr %2, ptr %6, align 8
  %8 = call noalias ptr @calloc(i64 noundef 1, i64 noundef 80) #5
  store ptr %8, ptr %7, align 8
  %9 = load i32, ptr %4, align 4
  %10 = load ptr, ptr %7, align 8
  %11 = getelementptr inbounds %struct.Node, ptr %10, i32 0, i32 10
  store i32 %9, ptr %11, align 8
  %12 = load ptr, ptr %5, align 8
  %13 = load ptr, ptr %7, align 8
  %14 = getelementptr inbounds %struct.Node, ptr %13, i32 0, i32 0
  store ptr %12, ptr %14, align 8
  %15 = load ptr, ptr %6, align 8
  %16 = load ptr, ptr %7, align 8
  %17 = getelementptr inbounds %struct.Node, ptr %16, i32 0, i32 1
  store ptr %15, ptr %17, align 8
  %18 = load ptr, ptr %7, align 8
  ret ptr %18
}

; Function Attrs: nounwind allocsize(0,1)
declare noalias ptr @calloc(i64 noundef, i64 noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @new_node_num(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca ptr, align 8
  store i32 %0, ptr %2, align 4
  %4 = call noalias ptr @calloc(i64 noundef 1, i64 noundef 80) #5
  store ptr %4, ptr %3, align 8
  %5 = load ptr, ptr %3, align 8
  %6 = getelementptr inbounds %struct.Node, ptr %5, i32 0, i32 10
  store i32 31, ptr %6, align 8
  %7 = load i32, ptr %2, align 4
  %8 = load ptr, ptr %3, align 8
  %9 = getelementptr inbounds %struct.Node, ptr %8, i32 0, i32 8
  store i32 %7, ptr %9, align 8
  %10 = load ptr, ptr %3, align 8
  ret ptr %10
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @print_node(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  %11 = call ptr @init_code()
  store ptr %11, ptr %4, align 8
  %12 = load ptr, ptr %3, align 8
  %13 = icmp eq ptr %12, null
  br i1 %13, label %14, label %16

14:                                               ; preds = %1
  %15 = load ptr, ptr %4, align 8
  store ptr %15, ptr %2, align 8
  br label %590

16:                                               ; preds = %1
  %17 = load ptr, ptr %3, align 8
  %18 = getelementptr inbounds %struct.Node, ptr %17, i32 0, i32 7
  %19 = load ptr, ptr %18, align 8
  %20 = icmp ne ptr %19, null
  br i1 %20, label %21, label %29

21:                                               ; preds = %16
  %22 = load ptr, ptr %3, align 8
  %23 = getelementptr inbounds %struct.Node, ptr %22, i32 0, i32 7
  %24 = load ptr, ptr %23, align 8
  %25 = call ptr @get_variable_type_str(ptr noundef %24)
  store ptr %25, ptr %5, align 8
  %26 = load ptr, ptr %4, align 8
  %27 = load ptr, ptr %5, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %26, ptr noundef @.str, ptr noundef %27)
  %28 = load ptr, ptr %5, align 8
  call void @free(ptr noundef %28) #6
  br label %29

29:                                               ; preds = %21, %16
  %30 = load ptr, ptr %3, align 8
  %31 = getelementptr inbounds %struct.Node, ptr %30, i32 0, i32 10
  %32 = load i32, ptr %31, align 8
  %33 = icmp eq i32 %32, 31
  br i1 %33, label %34, label %40

34:                                               ; preds = %29
  %35 = load ptr, ptr %4, align 8
  %36 = load ptr, ptr %3, align 8
  %37 = getelementptr inbounds %struct.Node, ptr %36, i32 0, i32 8
  %38 = load i32, ptr %37, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %35, ptr noundef @.str.1, i32 noundef %38)
  %39 = load ptr, ptr %4, align 8
  store ptr %39, ptr %2, align 8
  br label %590

40:                                               ; preds = %29
  %41 = load ptr, ptr %3, align 8
  %42 = getelementptr inbounds %struct.Node, ptr %41, i32 0, i32 10
  %43 = load i32, ptr %42, align 8
  %44 = icmp eq i32 %43, 18
  br i1 %44, label %45, label %82

45:                                               ; preds = %40
  %46 = load ptr, ptr %3, align 8
  %47 = getelementptr inbounds %struct.Node, ptr %46, i32 0, i32 9
  %48 = load i32, ptr %47, align 4
  store i32 %48, ptr %6, align 4
  %49 = load ptr, ptr %3, align 8
  %50 = getelementptr inbounds %struct.Node, ptr %49, i32 0, i32 9
  %51 = load i32, ptr %50, align 4
  %52 = icmp slt i32 %51, 0
  br i1 %52, label %53, label %60

53:                                               ; preds = %45
  %54 = load ptr, ptr %3, align 8
  %55 = getelementptr inbounds %struct.Node, ptr %54, i32 0, i32 9
  %56 = load i32, ptr %55, align 4
  %57 = sub nsw i32 0, %56
  %58 = sub nsw i32 %57, 1
  %59 = sub nsw i32 %58, 32
  store i32 %59, ptr %6, align 4
  br label %60

60:                                               ; preds = %53, %45
  %61 = load ptr, ptr %4, align 8
  %62 = load i32, ptr %6, align 4
  %63 = add nsw i32 97, %62
  call void (ptr, ptr, ...) @push_code(ptr noundef %61, ptr noundef @.str.2, i32 noundef %63)
  %64 = load ptr, ptr %3, align 8
  %65 = getelementptr inbounds %struct.Node, ptr %64, i32 0, i32 10
  %66 = load i32, ptr %65, align 8
  %67 = icmp eq i32 %66, 17
  br i1 %67, label %68, label %80

68:                                               ; preds = %60
  %69 = load ptr, ptr %4, align 8
  %70 = load ptr, ptr %3, align 8
  %71 = getelementptr inbounds %struct.Node, ptr %70, i32 0, i32 0
  %72 = load ptr, ptr %71, align 8
  %73 = call ptr @print_node(ptr noundef %72)
  call void @merge_code(ptr noundef %69, ptr noundef %73)
  %74 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %74, ptr noundef @.str.3)
  %75 = load ptr, ptr %4, align 8
  %76 = load ptr, ptr %3, align 8
  %77 = getelementptr inbounds %struct.Node, ptr %76, i32 0, i32 1
  %78 = load ptr, ptr %77, align 8
  %79 = call ptr @print_node(ptr noundef %78)
  call void @merge_code(ptr noundef %75, ptr noundef %79)
  br label %80

80:                                               ; preds = %68, %60
  %81 = load ptr, ptr %4, align 8
  store ptr %81, ptr %2, align 8
  br label %590

82:                                               ; preds = %40
  %83 = load ptr, ptr %3, align 8
  %84 = getelementptr inbounds %struct.Node, ptr %83, i32 0, i32 10
  %85 = load i32, ptr %84, align 8
  %86 = icmp eq i32 %85, 19
  br i1 %86, label %87, label %95

87:                                               ; preds = %82
  %88 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %88, ptr noundef @.str.4)
  %89 = load ptr, ptr %4, align 8
  %90 = load ptr, ptr %3, align 8
  %91 = getelementptr inbounds %struct.Node, ptr %90, i32 0, i32 0
  %92 = load ptr, ptr %91, align 8
  %93 = call ptr @print_node(ptr noundef %92)
  call void @merge_code(ptr noundef %89, ptr noundef %93)
  %94 = load ptr, ptr %4, align 8
  store ptr %94, ptr %2, align 8
  br label %590

95:                                               ; preds = %82
  %96 = load ptr, ptr %3, align 8
  %97 = getelementptr inbounds %struct.Node, ptr %96, i32 0, i32 10
  %98 = load i32, ptr %97, align 8
  %99 = icmp eq i32 %98, 20
  br i1 %99, label %100, label %126

100:                                              ; preds = %95
  %101 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %101, ptr noundef @.str.5)
  %102 = load ptr, ptr %4, align 8
  %103 = load ptr, ptr %3, align 8
  %104 = getelementptr inbounds %struct.Node, ptr %103, i32 0, i32 0
  %105 = load ptr, ptr %104, align 8
  %106 = call ptr @print_node(ptr noundef %105)
  call void @merge_code(ptr noundef %102, ptr noundef %106)
  %107 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %107, ptr noundef @.str.6)
  %108 = load ptr, ptr %4, align 8
  %109 = load ptr, ptr %3, align 8
  %110 = getelementptr inbounds %struct.Node, ptr %109, i32 0, i32 1
  %111 = load ptr, ptr %110, align 8
  %112 = call ptr @print_node(ptr noundef %111)
  call void @merge_code(ptr noundef %108, ptr noundef %112)
  %113 = load ptr, ptr %3, align 8
  %114 = getelementptr inbounds %struct.Node, ptr %113, i32 0, i32 2
  %115 = load ptr, ptr %114, align 8
  %116 = icmp ne ptr %115, null
  br i1 %116, label %117, label %124

117:                                              ; preds = %100
  %118 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %118, ptr noundef @.str.7)
  %119 = load ptr, ptr %4, align 8
  %120 = load ptr, ptr %3, align 8
  %121 = getelementptr inbounds %struct.Node, ptr %120, i32 0, i32 2
  %122 = load ptr, ptr %121, align 8
  %123 = call ptr @print_node(ptr noundef %122)
  call void @merge_code(ptr noundef %119, ptr noundef %123)
  br label %124

124:                                              ; preds = %117, %100
  %125 = load ptr, ptr %4, align 8
  store ptr %125, ptr %2, align 8
  br label %590

126:                                              ; preds = %95
  %127 = load ptr, ptr %3, align 8
  %128 = getelementptr inbounds %struct.Node, ptr %127, i32 0, i32 10
  %129 = load i32, ptr %128, align 8
  %130 = icmp eq i32 %129, 21
  br i1 %130, label %131, label %145

131:                                              ; preds = %126
  %132 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %132, ptr noundef @.str.8)
  %133 = load ptr, ptr %4, align 8
  %134 = load ptr, ptr %3, align 8
  %135 = getelementptr inbounds %struct.Node, ptr %134, i32 0, i32 0
  %136 = load ptr, ptr %135, align 8
  %137 = call ptr @print_node(ptr noundef %136)
  call void @merge_code(ptr noundef %133, ptr noundef %137)
  %138 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %138, ptr noundef @.str.6)
  %139 = load ptr, ptr %4, align 8
  %140 = load ptr, ptr %3, align 8
  %141 = getelementptr inbounds %struct.Node, ptr %140, i32 0, i32 1
  %142 = load ptr, ptr %141, align 8
  %143 = call ptr @print_node(ptr noundef %142)
  call void @merge_code(ptr noundef %139, ptr noundef %143)
  %144 = load ptr, ptr %4, align 8
  store ptr %144, ptr %2, align 8
  br label %590

145:                                              ; preds = %126
  %146 = load ptr, ptr %3, align 8
  %147 = getelementptr inbounds %struct.Node, ptr %146, i32 0, i32 10
  %148 = load i32, ptr %147, align 8
  %149 = icmp eq i32 %148, 22
  br i1 %149, label %150, label %186

150:                                              ; preds = %145
  %151 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %151, ptr noundef @.str.9)
  %152 = load ptr, ptr %3, align 8
  %153 = getelementptr inbounds %struct.Node, ptr %152, i32 0, i32 0
  %154 = load ptr, ptr %153, align 8
  %155 = icmp ne ptr %154, null
  br i1 %155, label %156, label %161

156:                                              ; preds = %150
  %157 = load ptr, ptr %3, align 8
  %158 = getelementptr inbounds %struct.Node, ptr %157, i32 0, i32 0
  %159 = load ptr, ptr %158, align 8
  %160 = call ptr @print_node(ptr noundef %159)
  br label %161

161:                                              ; preds = %156, %150
  %162 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %162, ptr noundef @.str.10)
  %163 = load ptr, ptr %3, align 8
  %164 = getelementptr inbounds %struct.Node, ptr %163, i32 0, i32 1
  %165 = load ptr, ptr %164, align 8
  %166 = icmp ne ptr %165, null
  br i1 %166, label %167, label %172

167:                                              ; preds = %161
  %168 = load ptr, ptr %3, align 8
  %169 = getelementptr inbounds %struct.Node, ptr %168, i32 0, i32 1
  %170 = load ptr, ptr %169, align 8
  %171 = call ptr @print_node(ptr noundef %170)
  br label %172

172:                                              ; preds = %167, %161
  %173 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %173, ptr noundef @.str.10)
  %174 = load ptr, ptr %3, align 8
  %175 = getelementptr inbounds %struct.Node, ptr %174, i32 0, i32 2
  %176 = load ptr, ptr %175, align 8
  %177 = icmp ne ptr %176, null
  br i1 %177, label %178, label %183

178:                                              ; preds = %172
  %179 = load ptr, ptr %3, align 8
  %180 = getelementptr inbounds %struct.Node, ptr %179, i32 0, i32 2
  %181 = load ptr, ptr %180, align 8
  %182 = call ptr @print_node(ptr noundef %181)
  br label %183

183:                                              ; preds = %178, %172
  %184 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %184, ptr noundef @.str.11)
  %185 = load ptr, ptr %4, align 8
  store ptr %185, ptr %2, align 8
  br label %590

186:                                              ; preds = %145
  %187 = load ptr, ptr %3, align 8
  %188 = getelementptr inbounds %struct.Node, ptr %187, i32 0, i32 10
  %189 = load i32, ptr %188, align 8
  %190 = icmp eq i32 %189, 28
  br i1 %190, label %191, label %216

191:                                              ; preds = %186
  %192 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %192, ptr noundef @.str.12)
  store i32 0, ptr %7, align 4
  br label %193

193:                                              ; preds = %210, %191
  %194 = load i32, ptr %7, align 4
  %195 = load ptr, ptr %3, align 8
  %196 = getelementptr inbounds %struct.Node, ptr %195, i32 0, i32 5
  %197 = load ptr, ptr %196, align 8
  %198 = getelementptr inbounds %struct.vector, ptr %197, i32 0, i32 1
  %199 = load i32, ptr %198, align 8
  %200 = icmp slt i32 %194, %199
  br i1 %200, label %201, label %213

201:                                              ; preds = %193
  %202 = load ptr, ptr %4, align 8
  %203 = load ptr, ptr %3, align 8
  %204 = getelementptr inbounds %struct.Node, ptr %203, i32 0, i32 5
  %205 = load ptr, ptr %204, align 8
  %206 = load i32, ptr %7, align 4
  %207 = call ptr @vec_at(ptr noundef %205, i32 noundef %206)
  %208 = call ptr @print_node(ptr noundef %207)
  call void @merge_code(ptr noundef %202, ptr noundef %208)
  %209 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %209, ptr noundef @.str.13)
  br label %210

210:                                              ; preds = %201
  %211 = load i32, ptr %7, align 4
  %212 = add nsw i32 %211, 1
  store i32 %212, ptr %7, align 4
  br label %193, !llvm.loop !6

213:                                              ; preds = %193
  %214 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %214, ptr noundef @.str.14)
  %215 = load ptr, ptr %4, align 8
  store ptr %215, ptr %2, align 8
  br label %590

216:                                              ; preds = %186
  %217 = load ptr, ptr %3, align 8
  %218 = getelementptr inbounds %struct.Node, ptr %217, i32 0, i32 10
  %219 = load i32, ptr %218, align 8
  %220 = icmp eq i32 %219, 29
  br i1 %220, label %221, label %244

221:                                              ; preds = %216
  store i32 0, ptr %8, align 4
  br label %222

222:                                              ; preds = %239, %221
  %223 = load i32, ptr %8, align 4
  %224 = load ptr, ptr %3, align 8
  %225 = getelementptr inbounds %struct.Node, ptr %224, i32 0, i32 5
  %226 = load ptr, ptr %225, align 8
  %227 = getelementptr inbounds %struct.vector, ptr %226, i32 0, i32 1
  %228 = load i32, ptr %227, align 8
  %229 = icmp slt i32 %223, %228
  br i1 %229, label %230, label %242

230:                                              ; preds = %222
  %231 = load ptr, ptr %4, align 8
  %232 = load ptr, ptr %3, align 8
  %233 = getelementptr inbounds %struct.Node, ptr %232, i32 0, i32 5
  %234 = load ptr, ptr %233, align 8
  %235 = load i32, ptr %8, align 4
  %236 = call ptr @vec_at(ptr noundef %234, i32 noundef %235)
  %237 = call ptr @print_node(ptr noundef %236)
  call void @merge_code(ptr noundef %231, ptr noundef %237)
  %238 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %238, ptr noundef @.str.13)
  br label %239

239:                                              ; preds = %230
  %240 = load i32, ptr %8, align 4
  %241 = add nsw i32 %240, 1
  store i32 %241, ptr %8, align 4
  br label %222, !llvm.loop !8

242:                                              ; preds = %222
  %243 = load ptr, ptr %4, align 8
  store ptr %243, ptr %2, align 8
  br label %590

244:                                              ; preds = %216
  %245 = load ptr, ptr %3, align 8
  %246 = getelementptr inbounds %struct.Node, ptr %245, i32 0, i32 10
  %247 = load i32, ptr %246, align 8
  %248 = icmp eq i32 %247, 30
  br i1 %248, label %249, label %292

249:                                              ; preds = %244
  %250 = load ptr, ptr %4, align 8
  %251 = load ptr, ptr %3, align 8
  %252 = getelementptr inbounds %struct.Node, ptr %251, i32 0, i32 4
  %253 = load ptr, ptr %252, align 8
  %254 = getelementptr inbounds %struct.Call, ptr %253, i32 0, i32 1
  %255 = load i32, ptr %254, align 8
  %256 = load ptr, ptr %3, align 8
  %257 = getelementptr inbounds %struct.Node, ptr %256, i32 0, i32 4
  %258 = load ptr, ptr %257, align 8
  %259 = getelementptr inbounds %struct.Call, ptr %258, i32 0, i32 0
  %260 = load ptr, ptr %259, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %250, ptr noundef @.str.15, i32 noundef %255, ptr noundef %260)
  store i32 0, ptr %9, align 4
  br label %261

261:                                              ; preds = %286, %249
  %262 = load i32, ptr %9, align 4
  %263 = load ptr, ptr %3, align 8
  %264 = getelementptr inbounds %struct.Node, ptr %263, i32 0, i32 4
  %265 = load ptr, ptr %264, align 8
  %266 = getelementptr inbounds %struct.Call, ptr %265, i32 0, i32 3
  %267 = load ptr, ptr %266, align 8
  %268 = getelementptr inbounds %struct.vector, ptr %267, i32 0, i32 1
  %269 = load i32, ptr %268, align 8
  %270 = icmp slt i32 %262, %269
  br i1 %270, label %271, label %289

271:                                              ; preds = %261
  %272 = load i32, ptr %9, align 4
  %273 = icmp sgt i32 %272, 0
  br i1 %273, label %274, label %276

274:                                              ; preds = %271
  %275 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %275, ptr noundef @.str.16)
  br label %276

276:                                              ; preds = %274, %271
  %277 = load ptr, ptr %4, align 8
  %278 = load ptr, ptr %3, align 8
  %279 = getelementptr inbounds %struct.Node, ptr %278, i32 0, i32 4
  %280 = load ptr, ptr %279, align 8
  %281 = getelementptr inbounds %struct.Call, ptr %280, i32 0, i32 3
  %282 = load ptr, ptr %281, align 8
  %283 = load i32, ptr %9, align 4
  %284 = call ptr @vec_at(ptr noundef %282, i32 noundef %283)
  %285 = call ptr @print_node(ptr noundef %284)
  call void @merge_code(ptr noundef %277, ptr noundef %285)
  br label %286

286:                                              ; preds = %276
  %287 = load i32, ptr %9, align 4
  %288 = add nsw i32 %287, 1
  store i32 %288, ptr %9, align 4
  br label %261, !llvm.loop !9

289:                                              ; preds = %261
  %290 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %290, ptr noundef @.str.11)
  %291 = load ptr, ptr %4, align 8
  store ptr %291, ptr %2, align 8
  br label %590

292:                                              ; preds = %244
  %293 = load ptr, ptr %3, align 8
  %294 = getelementptr inbounds %struct.Node, ptr %293, i32 0, i32 10
  %295 = load i32, ptr %294, align 8
  %296 = icmp eq i32 %295, 12
  br i1 %296, label %297, label %305

297:                                              ; preds = %292
  %298 = load ptr, ptr %4, align 8
  %299 = load ptr, ptr %3, align 8
  %300 = getelementptr inbounds %struct.Node, ptr %299, i32 0, i32 0
  %301 = load ptr, ptr %300, align 8
  %302 = call ptr @print_node(ptr noundef %301)
  call void @merge_code(ptr noundef %298, ptr noundef %302)
  %303 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %303, ptr noundef @.str.17)
  %304 = load ptr, ptr %4, align 8
  store ptr %304, ptr %2, align 8
  br label %590

305:                                              ; preds = %292
  %306 = load ptr, ptr %3, align 8
  %307 = getelementptr inbounds %struct.Node, ptr %306, i32 0, i32 10
  %308 = load i32, ptr %307, align 8
  %309 = icmp eq i32 %308, 13
  br i1 %309, label %310, label %318

310:                                              ; preds = %305
  %311 = load ptr, ptr %4, align 8
  %312 = load ptr, ptr %3, align 8
  %313 = getelementptr inbounds %struct.Node, ptr %312, i32 0, i32 0
  %314 = load ptr, ptr %313, align 8
  %315 = call ptr @print_node(ptr noundef %314)
  call void @merge_code(ptr noundef %311, ptr noundef %315)
  %316 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %316, ptr noundef @.str.18)
  %317 = load ptr, ptr %4, align 8
  store ptr %317, ptr %2, align 8
  br label %590

318:                                              ; preds = %305
  %319 = load ptr, ptr %3, align 8
  %320 = getelementptr inbounds %struct.Node, ptr %319, i32 0, i32 10
  %321 = load i32, ptr %320, align 8
  %322 = icmp eq i32 %321, 14
  br i1 %322, label %323, label %331

323:                                              ; preds = %318
  %324 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %324, ptr noundef @.str.19)
  %325 = load ptr, ptr %4, align 8
  %326 = load ptr, ptr %3, align 8
  %327 = getelementptr inbounds %struct.Node, ptr %326, i32 0, i32 0
  %328 = load ptr, ptr %327, align 8
  %329 = call ptr @print_node(ptr noundef %328)
  call void @merge_code(ptr noundef %325, ptr noundef %329)
  %330 = load ptr, ptr %4, align 8
  store ptr %330, ptr %2, align 8
  br label %590

331:                                              ; preds = %318
  %332 = load ptr, ptr %3, align 8
  %333 = getelementptr inbounds %struct.Node, ptr %332, i32 0, i32 10
  %334 = load i32, ptr %333, align 8
  %335 = icmp eq i32 %334, 15
  br i1 %335, label %336, label %344

336:                                              ; preds = %331
  %337 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %337, ptr noundef @.str.20)
  %338 = load ptr, ptr %4, align 8
  %339 = load ptr, ptr %3, align 8
  %340 = getelementptr inbounds %struct.Node, ptr %339, i32 0, i32 0
  %341 = load ptr, ptr %340, align 8
  %342 = call ptr @print_node(ptr noundef %341)
  call void @merge_code(ptr noundef %338, ptr noundef %342)
  %343 = load ptr, ptr %4, align 8
  store ptr %343, ptr %2, align 8
  br label %590

344:                                              ; preds = %331
  %345 = load ptr, ptr %3, align 8
  %346 = getelementptr inbounds %struct.Node, ptr %345, i32 0, i32 10
  %347 = load i32, ptr %346, align 8
  %348 = icmp eq i32 %347, 16
  br i1 %348, label %349, label %362

349:                                              ; preds = %344
  %350 = load ptr, ptr %4, align 8
  %351 = load ptr, ptr %3, align 8
  %352 = getelementptr inbounds %struct.Node, ptr %351, i32 0, i32 0
  %353 = load ptr, ptr %352, align 8
  %354 = call ptr @print_node(ptr noundef %353)
  call void @merge_code(ptr noundef %350, ptr noundef %354)
  %355 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %355, ptr noundef @.str.21)
  %356 = load ptr, ptr %4, align 8
  %357 = load ptr, ptr %3, align 8
  %358 = getelementptr inbounds %struct.Node, ptr %357, i32 0, i32 1
  %359 = load ptr, ptr %358, align 8
  %360 = call ptr @print_node(ptr noundef %359)
  call void @merge_code(ptr noundef %356, ptr noundef %360)
  %361 = load ptr, ptr %4, align 8
  store ptr %361, ptr %2, align 8
  br label %590

362:                                              ; preds = %344
  %363 = load ptr, ptr %3, align 8
  %364 = getelementptr inbounds %struct.Node, ptr %363, i32 0, i32 10
  %365 = load i32, ptr %364, align 8
  %366 = icmp eq i32 %365, 33
  br i1 %366, label %367, label %390

367:                                              ; preds = %362
  %368 = load ptr, ptr %4, align 8
  %369 = load ptr, ptr %3, align 8
  %370 = getelementptr inbounds %struct.Node, ptr %369, i32 0, i32 0
  %371 = load ptr, ptr %370, align 8
  %372 = call ptr @print_node(ptr noundef %371)
  call void @merge_code(ptr noundef %368, ptr noundef %372)
  %373 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %373, ptr noundef @.str.22)
  %374 = load ptr, ptr %4, align 8
  %375 = load ptr, ptr %3, align 8
  %376 = getelementptr inbounds %struct.Node, ptr %375, i32 0, i32 1
  %377 = load ptr, ptr %376, align 8
  %378 = getelementptr inbounds %struct.Node, ptr %377, i32 0, i32 0
  %379 = load ptr, ptr %378, align 8
  %380 = call ptr @print_node(ptr noundef %379)
  call void @merge_code(ptr noundef %374, ptr noundef %380)
  %381 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %381, ptr noundef @.str.23)
  %382 = load ptr, ptr %4, align 8
  %383 = load ptr, ptr %3, align 8
  %384 = getelementptr inbounds %struct.Node, ptr %383, i32 0, i32 1
  %385 = load ptr, ptr %384, align 8
  %386 = getelementptr inbounds %struct.Node, ptr %385, i32 0, i32 1
  %387 = load ptr, ptr %386, align 8
  %388 = call ptr @print_node(ptr noundef %387)
  call void @merge_code(ptr noundef %382, ptr noundef %388)
  %389 = load ptr, ptr %4, align 8
  store ptr %389, ptr %2, align 8
  br label %590

390:                                              ; preds = %362
  %391 = load ptr, ptr %3, align 8
  %392 = getelementptr inbounds %struct.Node, ptr %391, i32 0, i32 10
  %393 = load i32, ptr %392, align 8
  %394 = icmp eq i32 %393, 23
  br i1 %394, label %395, label %398

395:                                              ; preds = %390
  %396 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %396, ptr noundef @.str.24)
  %397 = load ptr, ptr %4, align 8
  store ptr %397, ptr %2, align 8
  br label %590

398:                                              ; preds = %390
  %399 = load ptr, ptr %3, align 8
  %400 = getelementptr inbounds %struct.Node, ptr %399, i32 0, i32 10
  %401 = load i32, ptr %400, align 8
  %402 = icmp eq i32 %401, 24
  br i1 %402, label %403, label %406

403:                                              ; preds = %398
  %404 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %404, ptr noundef @.str.25)
  %405 = load ptr, ptr %4, align 8
  store ptr %405, ptr %2, align 8
  br label %590

406:                                              ; preds = %398
  %407 = load ptr, ptr %3, align 8
  %408 = getelementptr inbounds %struct.Node, ptr %407, i32 0, i32 10
  %409 = load i32, ptr %408, align 8
  %410 = icmp eq i32 %409, 25
  br i1 %410, label %411, label %441

411:                                              ; preds = %406
  %412 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %412, ptr noundef @.str.26)
  %413 = load ptr, ptr %4, align 8
  %414 = load ptr, ptr %3, align 8
  %415 = getelementptr inbounds %struct.Node, ptr %414, i32 0, i32 0
  %416 = load ptr, ptr %415, align 8
  %417 = call ptr @print_node(ptr noundef %416)
  call void @merge_code(ptr noundef %413, ptr noundef %417)
  %418 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %418, ptr noundef @.str.27)
  store i32 0, ptr %10, align 4
  br label %419

419:                                              ; preds = %435, %411
  %420 = load i32, ptr %10, align 4
  %421 = load ptr, ptr %3, align 8
  %422 = getelementptr inbounds %struct.Node, ptr %421, i32 0, i32 5
  %423 = load ptr, ptr %422, align 8
  %424 = getelementptr inbounds %struct.vector, ptr %423, i32 0, i32 1
  %425 = load i32, ptr %424, align 8
  %426 = icmp slt i32 %420, %425
  br i1 %426, label %427, label %438

427:                                              ; preds = %419
  %428 = load ptr, ptr %4, align 8
  %429 = load ptr, ptr %3, align 8
  %430 = getelementptr inbounds %struct.Node, ptr %429, i32 0, i32 5
  %431 = load ptr, ptr %430, align 8
  %432 = load i32, ptr %10, align 4
  %433 = call ptr @vec_at(ptr noundef %431, i32 noundef %432)
  %434 = call ptr @print_node(ptr noundef %433)
  call void @merge_code(ptr noundef %428, ptr noundef %434)
  br label %435

435:                                              ; preds = %427
  %436 = load i32, ptr %10, align 4
  %437 = add nsw i32 %436, 1
  store i32 %437, ptr %10, align 4
  br label %419, !llvm.loop !10

438:                                              ; preds = %419
  %439 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %439, ptr noundef @.str.14)
  %440 = load ptr, ptr %4, align 8
  store ptr %440, ptr %2, align 8
  br label %590

441:                                              ; preds = %406
  %442 = load ptr, ptr %3, align 8
  %443 = getelementptr inbounds %struct.Node, ptr %442, i32 0, i32 10
  %444 = load i32, ptr %443, align 8
  %445 = icmp eq i32 %444, 26
  br i1 %445, label %446, label %460

446:                                              ; preds = %441
  %447 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %447, ptr noundef @.str.28)
  %448 = load ptr, ptr %4, align 8
  %449 = load ptr, ptr %3, align 8
  %450 = getelementptr inbounds %struct.Node, ptr %449, i32 0, i32 0
  %451 = load ptr, ptr %450, align 8
  %452 = call ptr @print_node(ptr noundef %451)
  call void @merge_code(ptr noundef %448, ptr noundef %452)
  %453 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %453, ptr noundef @.str.29)
  %454 = load ptr, ptr %4, align 8
  %455 = load ptr, ptr %3, align 8
  %456 = getelementptr inbounds %struct.Node, ptr %455, i32 0, i32 1
  %457 = load ptr, ptr %456, align 8
  %458 = call ptr @print_node(ptr noundef %457)
  call void @merge_code(ptr noundef %454, ptr noundef %458)
  %459 = load ptr, ptr %4, align 8
  store ptr %459, ptr %2, align 8
  br label %590

460:                                              ; preds = %441
  %461 = load ptr, ptr %3, align 8
  %462 = getelementptr inbounds %struct.Node, ptr %461, i32 0, i32 10
  %463 = load i32, ptr %462, align 8
  %464 = icmp eq i32 %463, 27
  br i1 %464, label %465, label %473

465:                                              ; preds = %460
  %466 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %466, ptr noundef @.str.30)
  %467 = load ptr, ptr %4, align 8
  %468 = load ptr, ptr %3, align 8
  %469 = getelementptr inbounds %struct.Node, ptr %468, i32 0, i32 1
  %470 = load ptr, ptr %469, align 8
  %471 = call ptr @print_node(ptr noundef %470)
  call void @merge_code(ptr noundef %467, ptr noundef %471)
  %472 = load ptr, ptr %4, align 8
  store ptr %472, ptr %2, align 8
  br label %590

473:                                              ; preds = %460
  %474 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %474, ptr noundef @.str.31)
  %475 = load ptr, ptr %4, align 8
  %476 = load ptr, ptr %3, align 8
  %477 = getelementptr inbounds %struct.Node, ptr %476, i32 0, i32 0
  %478 = load ptr, ptr %477, align 8
  %479 = call ptr @print_node(ptr noundef %478)
  call void @merge_code(ptr noundef %475, ptr noundef %479)
  %480 = load ptr, ptr %3, align 8
  %481 = getelementptr inbounds %struct.Node, ptr %480, i32 0, i32 10
  %482 = load i32, ptr %481, align 8
  %483 = icmp eq i32 %482, 0
  br i1 %483, label %484, label %486

484:                                              ; preds = %473
  %485 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %485, ptr noundef @.str.32)
  br label %582

486:                                              ; preds = %473
  %487 = load ptr, ptr %3, align 8
  %488 = getelementptr inbounds %struct.Node, ptr %487, i32 0, i32 10
  %489 = load i32, ptr %488, align 8
  %490 = icmp eq i32 %489, 1
  br i1 %490, label %491, label %493

491:                                              ; preds = %486
  %492 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %492, ptr noundef @.str.33)
  br label %581

493:                                              ; preds = %486
  %494 = load ptr, ptr %3, align 8
  %495 = getelementptr inbounds %struct.Node, ptr %494, i32 0, i32 10
  %496 = load i32, ptr %495, align 8
  %497 = icmp eq i32 %496, 2
  br i1 %497, label %498, label %500

498:                                              ; preds = %493
  %499 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %499, ptr noundef @.str.20)
  br label %580

500:                                              ; preds = %493
  %501 = load ptr, ptr %3, align 8
  %502 = getelementptr inbounds %struct.Node, ptr %501, i32 0, i32 10
  %503 = load i32, ptr %502, align 8
  %504 = icmp eq i32 %503, 3
  br i1 %504, label %505, label %507

505:                                              ; preds = %500
  %506 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %506, ptr noundef @.str.34)
  br label %579

507:                                              ; preds = %500
  %508 = load ptr, ptr %3, align 8
  %509 = getelementptr inbounds %struct.Node, ptr %508, i32 0, i32 10
  %510 = load i32, ptr %509, align 8
  %511 = icmp eq i32 %510, 4
  br i1 %511, label %512, label %514

512:                                              ; preds = %507
  %513 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %513, ptr noundef @.str.35)
  br label %578

514:                                              ; preds = %507
  %515 = load ptr, ptr %3, align 8
  %516 = getelementptr inbounds %struct.Node, ptr %515, i32 0, i32 10
  %517 = load i32, ptr %516, align 8
  %518 = icmp eq i32 %517, 5
  br i1 %518, label %519, label %521

519:                                              ; preds = %514
  %520 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %520, ptr noundef @.str.36)
  br label %577

521:                                              ; preds = %514
  %522 = load ptr, ptr %3, align 8
  %523 = getelementptr inbounds %struct.Node, ptr %522, i32 0, i32 10
  %524 = load i32, ptr %523, align 8
  %525 = icmp eq i32 %524, 6
  br i1 %525, label %526, label %528

526:                                              ; preds = %521
  %527 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %527, ptr noundef @.str.37)
  br label %576

528:                                              ; preds = %521
  %529 = load ptr, ptr %3, align 8
  %530 = getelementptr inbounds %struct.Node, ptr %529, i32 0, i32 10
  %531 = load i32, ptr %530, align 8
  %532 = icmp eq i32 %531, 7
  br i1 %532, label %533, label %535

533:                                              ; preds = %528
  %534 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %534, ptr noundef @.str.38)
  br label %575

535:                                              ; preds = %528
  %536 = load ptr, ptr %3, align 8
  %537 = getelementptr inbounds %struct.Node, ptr %536, i32 0, i32 10
  %538 = load i32, ptr %537, align 8
  %539 = icmp eq i32 %538, 8
  br i1 %539, label %540, label %542

540:                                              ; preds = %535
  %541 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %541, ptr noundef @.str.39)
  br label %574

542:                                              ; preds = %535
  %543 = load ptr, ptr %3, align 8
  %544 = getelementptr inbounds %struct.Node, ptr %543, i32 0, i32 10
  %545 = load i32, ptr %544, align 8
  %546 = icmp eq i32 %545, 9
  br i1 %546, label %547, label %549

547:                                              ; preds = %542
  %548 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %548, ptr noundef @.str.40)
  br label %573

549:                                              ; preds = %542
  %550 = load ptr, ptr %3, align 8
  %551 = getelementptr inbounds %struct.Node, ptr %550, i32 0, i32 10
  %552 = load i32, ptr %551, align 8
  %553 = icmp eq i32 %552, 17
  br i1 %553, label %554, label %556

554:                                              ; preds = %549
  %555 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %555, ptr noundef @.str.3)
  br label %572

556:                                              ; preds = %549
  %557 = load ptr, ptr %3, align 8
  %558 = getelementptr inbounds %struct.Node, ptr %557, i32 0, i32 10
  %559 = load i32, ptr %558, align 8
  %560 = icmp eq i32 %559, 10
  br i1 %560, label %561, label %563

561:                                              ; preds = %556
  %562 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %562, ptr noundef @.str.41)
  br label %571

563:                                              ; preds = %556
  %564 = load ptr, ptr %3, align 8
  %565 = getelementptr inbounds %struct.Node, ptr %564, i32 0, i32 10
  %566 = load i32, ptr %565, align 8
  %567 = icmp eq i32 %566, 11
  br i1 %567, label %568, label %570

568:                                              ; preds = %563
  %569 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %569, ptr noundef @.str.42)
  br label %570

570:                                              ; preds = %568, %563
  br label %571

571:                                              ; preds = %570, %561
  br label %572

572:                                              ; preds = %571, %554
  br label %573

573:                                              ; preds = %572, %547
  br label %574

574:                                              ; preds = %573, %540
  br label %575

575:                                              ; preds = %574, %533
  br label %576

576:                                              ; preds = %575, %526
  br label %577

577:                                              ; preds = %576, %519
  br label %578

578:                                              ; preds = %577, %512
  br label %579

579:                                              ; preds = %578, %505
  br label %580

580:                                              ; preds = %579, %498
  br label %581

581:                                              ; preds = %580, %491
  br label %582

582:                                              ; preds = %581, %484
  %583 = load ptr, ptr %4, align 8
  %584 = load ptr, ptr %3, align 8
  %585 = getelementptr inbounds %struct.Node, ptr %584, i32 0, i32 1
  %586 = load ptr, ptr %585, align 8
  %587 = call ptr @print_node(ptr noundef %586)
  call void @merge_code(ptr noundef %583, ptr noundef %587)
  %588 = load ptr, ptr %4, align 8
  call void (ptr, ptr, ...) @push_code(ptr noundef %588, ptr noundef @.str.11)
  %589 = load ptr, ptr %4, align 8
  store ptr %589, ptr %2, align 8
  br label %590

590:                                              ; preds = %582, %465, %446, %438, %403, %395, %367, %349, %336, %323, %310, %297, %289, %242, %213, %183, %131, %124, %87, %80, %34, %14
  %591 = load ptr, ptr %2, align 8
  ret ptr %591
}

declare ptr @init_code() #2

declare ptr @get_variable_type_str(ptr noundef) #2

declare void @push_code(ptr noundef, ptr noundef, ...) #2

; Function Attrs: nounwind
declare void @free(ptr noundef) #3

declare void @merge_code(ptr noundef, ptr noundef) #2

declare ptr @vec_at(ptr noundef, i32 noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @get_node_kind_name(i32 noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca i32, align 4
  store i32 %0, ptr %3, align 4
  %4 = load i32, ptr %3, align 4
  switch i32 %4, label %39 [
    i32 0, label %5
    i32 1, label %6
    i32 2, label %7
    i32 3, label %8
    i32 4, label %9
    i32 5, label %10
    i32 6, label %11
    i32 7, label %12
    i32 8, label %13
    i32 9, label %14
    i32 10, label %15
    i32 11, label %16
    i32 12, label %17
    i32 13, label %18
    i32 14, label %19
    i32 15, label %20
    i32 16, label %21
    i32 17, label %22
    i32 18, label %23
    i32 19, label %24
    i32 20, label %25
    i32 21, label %26
    i32 22, label %27
    i32 23, label %28
    i32 24, label %29
    i32 25, label %30
    i32 26, label %31
    i32 27, label %32
    i32 28, label %33
    i32 29, label %34
    i32 30, label %35
    i32 31, label %36
    i32 32, label %37
    i32 33, label %38
  ]

5:                                                ; preds = %1
  store ptr @.str.43, ptr %2, align 8
  br label %40

6:                                                ; preds = %1
  store ptr @.str.44, ptr %2, align 8
  br label %40

7:                                                ; preds = %1
  store ptr @.str.45, ptr %2, align 8
  br label %40

8:                                                ; preds = %1
  store ptr @.str.46, ptr %2, align 8
  br label %40

9:                                                ; preds = %1
  store ptr @.str.47, ptr %2, align 8
  br label %40

10:                                               ; preds = %1
  store ptr @.str.48, ptr %2, align 8
  br label %40

11:                                               ; preds = %1
  store ptr @.str.49, ptr %2, align 8
  br label %40

12:                                               ; preds = %1
  store ptr @.str.50, ptr %2, align 8
  br label %40

13:                                               ; preds = %1
  store ptr @.str.51, ptr %2, align 8
  br label %40

14:                                               ; preds = %1
  store ptr @.str.52, ptr %2, align 8
  br label %40

15:                                               ; preds = %1
  store ptr @.str.53, ptr %2, align 8
  br label %40

16:                                               ; preds = %1
  store ptr @.str.54, ptr %2, align 8
  br label %40

17:                                               ; preds = %1
  store ptr @.str.55, ptr %2, align 8
  br label %40

18:                                               ; preds = %1
  store ptr @.str.56, ptr %2, align 8
  br label %40

19:                                               ; preds = %1
  store ptr @.str.57, ptr %2, align 8
  br label %40

20:                                               ; preds = %1
  store ptr @.str.58, ptr %2, align 8
  br label %40

21:                                               ; preds = %1
  store ptr @.str.59, ptr %2, align 8
  br label %40

22:                                               ; preds = %1
  store ptr @.str.60, ptr %2, align 8
  br label %40

23:                                               ; preds = %1
  store ptr @.str.61, ptr %2, align 8
  br label %40

24:                                               ; preds = %1
  store ptr @.str.62, ptr %2, align 8
  br label %40

25:                                               ; preds = %1
  store ptr @.str.63, ptr %2, align 8
  br label %40

26:                                               ; preds = %1
  store ptr @.str.64, ptr %2, align 8
  br label %40

27:                                               ; preds = %1
  store ptr @.str.65, ptr %2, align 8
  br label %40

28:                                               ; preds = %1
  store ptr @.str.66, ptr %2, align 8
  br label %40

29:                                               ; preds = %1
  store ptr @.str.67, ptr %2, align 8
  br label %40

30:                                               ; preds = %1
  store ptr @.str.68, ptr %2, align 8
  br label %40

31:                                               ; preds = %1
  store ptr @.str.69, ptr %2, align 8
  br label %40

32:                                               ; preds = %1
  store ptr @.str.70, ptr %2, align 8
  br label %40

33:                                               ; preds = %1
  store ptr @.str.71, ptr %2, align 8
  br label %40

34:                                               ; preds = %1
  store ptr @.str.72, ptr %2, align 8
  br label %40

35:                                               ; preds = %1
  store ptr @.str.73, ptr %2, align 8
  br label %40

36:                                               ; preds = %1
  store ptr @.str.74, ptr %2, align 8
  br label %40

37:                                               ; preds = %1
  store ptr @.str.75, ptr %2, align 8
  br label %40

38:                                               ; preds = %1
  store ptr @.str.76, ptr %2, align 8
  br label %40

39:                                               ; preds = %1
  store ptr @.str.77, ptr %2, align 8
  br label %40

40:                                               ; preds = %39, %38, %37, %36, %35, %34, %33, %32, %31, %30, %29, %28, %27, %26, %25, %24, %23, %22, %21, %20, %19, %18, %17, %16, %15, %14, %13, %12, %11, %10, %9, %8, %7, %6, %5
  %41 = load ptr, ptr %2, align 8
  ret ptr %41
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @free_call(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca i32, align 4
  store ptr %0, ptr %2, align 8
  %4 = load ptr, ptr %2, align 8
  %5 = icmp eq ptr %4, null
  br i1 %5, label %6, label %7

6:                                                ; preds = %1
  br label %36

7:                                                ; preds = %1
  %8 = load ptr, ptr %2, align 8
  %9 = getelementptr inbounds %struct.Call, ptr %8, i32 0, i32 0
  %10 = load ptr, ptr %9, align 8
  call void @free(ptr noundef %10) #6
  %11 = load ptr, ptr %2, align 8
  %12 = getelementptr inbounds %struct.Call, ptr %11, i32 0, i32 2
  %13 = load ptr, ptr %12, align 8
  call void @free_variable(ptr noundef %13)
  store i32 0, ptr %3, align 4
  br label %14

14:                                               ; preds = %28, %7
  %15 = load i32, ptr %3, align 4
  %16 = load ptr, ptr %2, align 8
  %17 = getelementptr inbounds %struct.Call, ptr %16, i32 0, i32 3
  %18 = load ptr, ptr %17, align 8
  %19 = getelementptr inbounds %struct.vector, ptr %18, i32 0, i32 1
  %20 = load i32, ptr %19, align 8
  %21 = icmp slt i32 %15, %20
  br i1 %21, label %22, label %31

22:                                               ; preds = %14
  %23 = load ptr, ptr %2, align 8
  %24 = getelementptr inbounds %struct.Call, ptr %23, i32 0, i32 3
  %25 = load ptr, ptr %24, align 8
  %26 = load i32, ptr %3, align 4
  %27 = call ptr @vec_at(ptr noundef %25, i32 noundef %26)
  call void @free_node(ptr noundef %27)
  br label %28

28:                                               ; preds = %22
  %29 = load i32, ptr %3, align 4
  %30 = add nsw i32 %29, 1
  store i32 %30, ptr %3, align 4
  br label %14, !llvm.loop !11

31:                                               ; preds = %14
  %32 = load ptr, ptr %2, align 8
  %33 = getelementptr inbounds %struct.Call, ptr %32, i32 0, i32 3
  %34 = load ptr, ptr %33, align 8
  call void @vec_free(ptr noundef %34)
  %35 = load ptr, ptr %2, align 8
  call void @free(ptr noundef %35) #6
  br label %36

36:                                               ; preds = %31, %6
  ret void
}

declare void @free_variable(ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @free_node(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca i32, align 4
  store ptr %0, ptr %2, align 8
  %4 = load ptr, ptr %2, align 8
  %5 = icmp eq ptr %4, null
  br i1 %5, label %6, label %7

6:                                                ; preds = %1
  br label %51

7:                                                ; preds = %1
  %8 = load ptr, ptr %2, align 8
  %9 = getelementptr inbounds %struct.Node, ptr %8, i32 0, i32 0
  %10 = load ptr, ptr %9, align 8
  call void @free_node(ptr noundef %10)
  %11 = load ptr, ptr %2, align 8
  %12 = getelementptr inbounds %struct.Node, ptr %11, i32 0, i32 1
  %13 = load ptr, ptr %12, align 8
  call void @free_node(ptr noundef %13)
  %14 = load ptr, ptr %2, align 8
  %15 = getelementptr inbounds %struct.Node, ptr %14, i32 0, i32 2
  %16 = load ptr, ptr %15, align 8
  call void @free_node(ptr noundef %16)
  %17 = load ptr, ptr %2, align 8
  %18 = getelementptr inbounds %struct.Node, ptr %17, i32 0, i32 3
  %19 = load ptr, ptr %18, align 8
  call void @free_node(ptr noundef %19)
  %20 = load ptr, ptr %2, align 8
  %21 = getelementptr inbounds %struct.Node, ptr %20, i32 0, i32 4
  %22 = load ptr, ptr %21, align 8
  call void @free_call(ptr noundef %22)
  %23 = load ptr, ptr %2, align 8
  %24 = getelementptr inbounds %struct.Node, ptr %23, i32 0, i32 5
  %25 = load ptr, ptr %24, align 8
  %26 = icmp ne ptr %25, null
  br i1 %26, label %27, label %49

27:                                               ; preds = %7
  store i32 0, ptr %3, align 4
  br label %28

28:                                               ; preds = %42, %27
  %29 = load i32, ptr %3, align 4
  %30 = load ptr, ptr %2, align 8
  %31 = getelementptr inbounds %struct.Node, ptr %30, i32 0, i32 5
  %32 = load ptr, ptr %31, align 8
  %33 = getelementptr inbounds %struct.vector, ptr %32, i32 0, i32 1
  %34 = load i32, ptr %33, align 8
  %35 = icmp slt i32 %29, %34
  br i1 %35, label %36, label %45

36:                                               ; preds = %28
  %37 = load ptr, ptr %2, align 8
  %38 = getelementptr inbounds %struct.Node, ptr %37, i32 0, i32 5
  %39 = load ptr, ptr %38, align 8
  %40 = load i32, ptr %3, align 4
  %41 = call ptr @vec_at(ptr noundef %39, i32 noundef %40)
  call void @free_node(ptr noundef %41)
  br label %42

42:                                               ; preds = %36
  %43 = load i32, ptr %3, align 4
  %44 = add nsw i32 %43, 1
  store i32 %44, ptr %3, align 4
  br label %28, !llvm.loop !12

45:                                               ; preds = %28
  %46 = load ptr, ptr %2, align 8
  %47 = getelementptr inbounds %struct.Node, ptr %46, i32 0, i32 5
  %48 = load ptr, ptr %47, align 8
  call void @vec_free(ptr noundef %48)
  br label %49

49:                                               ; preds = %45, %7
  %50 = load ptr, ptr %2, align 8
  call void @free(ptr noundef %50) #6
  br label %51

51:                                               ; preds = %49, %6
  ret void
}

declare void @vec_free(ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @free_function(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store ptr %0, ptr %2, align 8
  %6 = load ptr, ptr %2, align 8
  %7 = icmp eq ptr %6, null
  br i1 %7, label %8, label %9

8:                                                ; preds = %1
  br label %80

9:                                                ; preds = %1
  %10 = load ptr, ptr %2, align 8
  %11 = getelementptr inbounds %struct.Function, ptr %10, i32 0, i32 0
  %12 = load ptr, ptr %11, align 8
  call void @free(ptr noundef %12) #6
  %13 = load ptr, ptr %2, align 8
  %14 = getelementptr inbounds %struct.Function, ptr %13, i32 0, i32 5
  %15 = load ptr, ptr %14, align 8
  call void @free_variable(ptr noundef %15)
  store i32 0, ptr %3, align 4
  br label %16

16:                                               ; preds = %30, %9
  %17 = load i32, ptr %3, align 4
  %18 = load ptr, ptr %2, align 8
  %19 = getelementptr inbounds %struct.Function, ptr %18, i32 0, i32 6
  %20 = load ptr, ptr %19, align 8
  %21 = getelementptr inbounds %struct.vector, ptr %20, i32 0, i32 1
  %22 = load i32, ptr %21, align 8
  %23 = icmp slt i32 %17, %22
  br i1 %23, label %24, label %33

24:                                               ; preds = %16
  %25 = load ptr, ptr %2, align 8
  %26 = getelementptr inbounds %struct.Function, ptr %25, i32 0, i32 6
  %27 = load ptr, ptr %26, align 8
  %28 = load i32, ptr %3, align 4
  %29 = call ptr @vec_at(ptr noundef %27, i32 noundef %28)
  call void @free_lvar(ptr noundef %29)
  br label %30

30:                                               ; preds = %24
  %31 = load i32, ptr %3, align 4
  %32 = add nsw i32 %31, 1
  store i32 %32, ptr %3, align 4
  br label %16, !llvm.loop !13

33:                                               ; preds = %16
  %34 = load ptr, ptr %2, align 8
  %35 = getelementptr inbounds %struct.Function, ptr %34, i32 0, i32 6
  %36 = load ptr, ptr %35, align 8
  call void @vec_free(ptr noundef %36)
  store i32 0, ptr %4, align 4
  br label %37

37:                                               ; preds = %51, %33
  %38 = load i32, ptr %4, align 4
  %39 = load ptr, ptr %2, align 8
  %40 = getelementptr inbounds %struct.Function, ptr %39, i32 0, i32 7
  %41 = load ptr, ptr %40, align 8
  %42 = getelementptr inbounds %struct.vector, ptr %41, i32 0, i32 1
  %43 = load i32, ptr %42, align 8
  %44 = icmp slt i32 %38, %43
  br i1 %44, label %45, label %54

45:                                               ; preds = %37
  %46 = load ptr, ptr %2, align 8
  %47 = getelementptr inbounds %struct.Function, ptr %46, i32 0, i32 7
  %48 = load ptr, ptr %47, align 8
  %49 = load i32, ptr %4, align 4
  %50 = call ptr @vec_at(ptr noundef %48, i32 noundef %49)
  call void @free_struct(ptr noundef %50)
  br label %51

51:                                               ; preds = %45
  %52 = load i32, ptr %4, align 4
  %53 = add nsw i32 %52, 1
  store i32 %53, ptr %4, align 4
  br label %37, !llvm.loop !14

54:                                               ; preds = %37
  %55 = load ptr, ptr %2, align 8
  %56 = getelementptr inbounds %struct.Function, ptr %55, i32 0, i32 7
  %57 = load ptr, ptr %56, align 8
  call void @vec_free(ptr noundef %57)
  store i32 0, ptr %5, align 4
  br label %58

58:                                               ; preds = %72, %54
  %59 = load i32, ptr %5, align 4
  %60 = load ptr, ptr %2, align 8
  %61 = getelementptr inbounds %struct.Function, ptr %60, i32 0, i32 8
  %62 = load ptr, ptr %61, align 8
  %63 = getelementptr inbounds %struct.vector, ptr %62, i32 0, i32 1
  %64 = load i32, ptr %63, align 8
  %65 = icmp slt i32 %59, %64
  br i1 %65, label %66, label %75

66:                                               ; preds = %58
  %67 = load ptr, ptr %2, align 8
  %68 = getelementptr inbounds %struct.Function, ptr %67, i32 0, i32 8
  %69 = load ptr, ptr %68, align 8
  %70 = load i32, ptr %5, align 4
  %71 = call ptr @vec_at(ptr noundef %69, i32 noundef %70)
  call void @free_node(ptr noundef %71)
  br label %72

72:                                               ; preds = %66
  %73 = load i32, ptr %5, align 4
  %74 = add nsw i32 %73, 1
  store i32 %74, ptr %5, align 4
  br label %58, !llvm.loop !15

75:                                               ; preds = %58
  %76 = load ptr, ptr %2, align 8
  %77 = getelementptr inbounds %struct.Function, ptr %76, i32 0, i32 8
  %78 = load ptr, ptr %77, align 8
  call void @vec_free(ptr noundef %78)
  %79 = load ptr, ptr %2, align 8
  call void @free(ptr noundef %79) #6
  br label %80

80:                                               ; preds = %75, %8
  ret void
}

declare void @free_lvar(ptr noundef) #2

declare void @free_struct(ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @copy_call(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  %6 = load ptr, ptr %3, align 8
  %7 = icmp eq ptr %6, null
  br i1 %7, label %8, label %9

8:                                                ; preds = %1
  store ptr null, ptr %2, align 8
  br label %72

9:                                                ; preds = %1
  %10 = call noalias ptr @calloc(i64 noundef 1, i64 noundef 32) #5
  store ptr %10, ptr %4, align 8
  %11 = load ptr, ptr %3, align 8
  %12 = getelementptr inbounds %struct.Call, ptr %11, i32 0, i32 1
  %13 = load i32, ptr %12, align 8
  %14 = add nsw i32 %13, 1
  %15 = sext i32 %14 to i64
  %16 = call noalias ptr @calloc(i64 noundef %15, i64 noundef 1) #5
  %17 = load ptr, ptr %4, align 8
  %18 = getelementptr inbounds %struct.Call, ptr %17, i32 0, i32 0
  store ptr %16, ptr %18, align 8
  %19 = load ptr, ptr %4, align 8
  %20 = getelementptr inbounds %struct.Call, ptr %19, i32 0, i32 0
  %21 = load ptr, ptr %20, align 8
  %22 = load ptr, ptr %3, align 8
  %23 = getelementptr inbounds %struct.Call, ptr %22, i32 0, i32 0
  %24 = load ptr, ptr %23, align 8
  %25 = load ptr, ptr %3, align 8
  %26 = getelementptr inbounds %struct.Call, ptr %25, i32 0, i32 1
  %27 = load i32, ptr %26, align 8
  %28 = sext i32 %27 to i64
  call void @llvm.memcpy.p0.p0.i64(ptr align 1 %21, ptr align 1 %24, i64 %28, i1 false)
  %29 = load ptr, ptr %3, align 8
  %30 = getelementptr inbounds %struct.Call, ptr %29, i32 0, i32 1
  %31 = load i32, ptr %30, align 8
  %32 = load ptr, ptr %4, align 8
  %33 = getelementptr inbounds %struct.Call, ptr %32, i32 0, i32 1
  store i32 %31, ptr %33, align 8
  %34 = load ptr, ptr %3, align 8
  %35 = getelementptr inbounds %struct.Call, ptr %34, i32 0, i32 2
  %36 = load ptr, ptr %35, align 8
  %37 = call ptr @copy_var(ptr noundef %36)
  %38 = load ptr, ptr %4, align 8
  %39 = getelementptr inbounds %struct.Call, ptr %38, i32 0, i32 2
  store ptr %37, ptr %39, align 8
  %40 = load ptr, ptr %3, align 8
  %41 = getelementptr inbounds %struct.Call, ptr %40, i32 0, i32 3
  %42 = load ptr, ptr %41, align 8
  %43 = icmp ne ptr %42, null
  br i1 %43, label %44, label %70

44:                                               ; preds = %9
  %45 = call ptr @new_vector()
  %46 = load ptr, ptr %4, align 8
  %47 = getelementptr inbounds %struct.Call, ptr %46, i32 0, i32 3
  store ptr %45, ptr %47, align 8
  store i32 0, ptr %5, align 4
  br label %48

48:                                               ; preds = %66, %44
  %49 = load i32, ptr %5, align 4
  %50 = load ptr, ptr %3, align 8
  %51 = getelementptr inbounds %struct.Call, ptr %50, i32 0, i32 3
  %52 = load ptr, ptr %51, align 8
  %53 = getelementptr inbounds %struct.vector, ptr %52, i32 0, i32 1
  %54 = load i32, ptr %53, align 8
  %55 = icmp slt i32 %49, %54
  br i1 %55, label %56, label %69

56:                                               ; preds = %48
  %57 = load ptr, ptr %4, align 8
  %58 = getelementptr inbounds %struct.Call, ptr %57, i32 0, i32 3
  %59 = load ptr, ptr %58, align 8
  %60 = load ptr, ptr %3, align 8
  %61 = getelementptr inbounds %struct.Call, ptr %60, i32 0, i32 3
  %62 = load ptr, ptr %61, align 8
  %63 = load i32, ptr %5, align 4
  %64 = call ptr @vec_at(ptr noundef %62, i32 noundef %63)
  %65 = call ptr @copy_node(ptr noundef %64)
  call void @vec_push_last(ptr noundef %59, ptr noundef %65)
  br label %66

66:                                               ; preds = %56
  %67 = load i32, ptr %5, align 4
  %68 = add nsw i32 %67, 1
  store i32 %68, ptr %5, align 4
  br label %48, !llvm.loop !16

69:                                               ; preds = %48
  br label %70

70:                                               ; preds = %69, %9
  %71 = load ptr, ptr %4, align 8
  store ptr %71, ptr %2, align 8
  br label %72

72:                                               ; preds = %70, %8
  %73 = load ptr, ptr %2, align 8
  ret ptr %73
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #4

declare ptr @copy_var(ptr noundef) #2

declare ptr @new_vector() #2

declare void @vec_push_last(ptr noundef, ptr noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local ptr @copy_node(ptr noundef %0) #0 {
  %2 = alloca ptr, align 8
  %3 = alloca ptr, align 8
  %4 = alloca ptr, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store ptr %0, ptr %3, align 8
  %7 = load ptr, ptr %3, align 8
  %8 = icmp eq ptr %7, null
  br i1 %8, label %9, label %10

9:                                                ; preds = %1
  store ptr null, ptr %2, align 8
  br label %126

10:                                               ; preds = %1
  %11 = call noalias ptr @calloc(i64 noundef 1, i64 noundef 80) #5
  store ptr %11, ptr %4, align 8
  %12 = load ptr, ptr %3, align 8
  %13 = getelementptr inbounds %struct.Node, ptr %12, i32 0, i32 10
  %14 = load i32, ptr %13, align 8
  %15 = load ptr, ptr %4, align 8
  %16 = getelementptr inbounds %struct.Node, ptr %15, i32 0, i32 10
  store i32 %14, ptr %16, align 8
  %17 = load ptr, ptr %3, align 8
  %18 = getelementptr inbounds %struct.Node, ptr %17, i32 0, i32 0
  %19 = load ptr, ptr %18, align 8
  %20 = call ptr @copy_node(ptr noundef %19)
  %21 = load ptr, ptr %4, align 8
  %22 = getelementptr inbounds %struct.Node, ptr %21, i32 0, i32 0
  store ptr %20, ptr %22, align 8
  %23 = load ptr, ptr %3, align 8
  %24 = getelementptr inbounds %struct.Node, ptr %23, i32 0, i32 1
  %25 = load ptr, ptr %24, align 8
  %26 = call ptr @copy_node(ptr noundef %25)
  %27 = load ptr, ptr %4, align 8
  %28 = getelementptr inbounds %struct.Node, ptr %27, i32 0, i32 1
  store ptr %26, ptr %28, align 8
  %29 = load ptr, ptr %3, align 8
  %30 = getelementptr inbounds %struct.Node, ptr %29, i32 0, i32 2
  %31 = load ptr, ptr %30, align 8
  %32 = call ptr @copy_node(ptr noundef %31)
  %33 = load ptr, ptr %4, align 8
  %34 = getelementptr inbounds %struct.Node, ptr %33, i32 0, i32 2
  store ptr %32, ptr %34, align 8
  %35 = load ptr, ptr %3, align 8
  %36 = getelementptr inbounds %struct.Node, ptr %35, i32 0, i32 3
  %37 = load ptr, ptr %36, align 8
  %38 = call ptr @copy_node(ptr noundef %37)
  %39 = load ptr, ptr %4, align 8
  %40 = getelementptr inbounds %struct.Node, ptr %39, i32 0, i32 3
  store ptr %38, ptr %40, align 8
  %41 = load ptr, ptr %3, align 8
  %42 = getelementptr inbounds %struct.Node, ptr %41, i32 0, i32 4
  %43 = load ptr, ptr %42, align 8
  %44 = call ptr @copy_call(ptr noundef %43)
  %45 = load ptr, ptr %4, align 8
  %46 = getelementptr inbounds %struct.Node, ptr %45, i32 0, i32 4
  store ptr %44, ptr %46, align 8
  %47 = load ptr, ptr %3, align 8
  %48 = getelementptr inbounds %struct.Node, ptr %47, i32 0, i32 5
  %49 = load ptr, ptr %48, align 8
  %50 = icmp ne ptr %49, null
  br i1 %50, label %51, label %77

51:                                               ; preds = %10
  %52 = call ptr @new_vector()
  %53 = load ptr, ptr %4, align 8
  %54 = getelementptr inbounds %struct.Node, ptr %53, i32 0, i32 5
  store ptr %52, ptr %54, align 8
  store i32 0, ptr %5, align 4
  br label %55

55:                                               ; preds = %73, %51
  %56 = load i32, ptr %5, align 4
  %57 = load ptr, ptr %3, align 8
  %58 = getelementptr inbounds %struct.Node, ptr %57, i32 0, i32 5
  %59 = load ptr, ptr %58, align 8
  %60 = getelementptr inbounds %struct.vector, ptr %59, i32 0, i32 1
  %61 = load i32, ptr %60, align 8
  %62 = icmp slt i32 %56, %61
  br i1 %62, label %63, label %76

63:                                               ; preds = %55
  %64 = load ptr, ptr %4, align 8
  %65 = getelementptr inbounds %struct.Node, ptr %64, i32 0, i32 5
  %66 = load ptr, ptr %65, align 8
  %67 = load ptr, ptr %3, align 8
  %68 = getelementptr inbounds %struct.Node, ptr %67, i32 0, i32 5
  %69 = load ptr, ptr %68, align 8
  %70 = load i32, ptr %5, align 4
  %71 = call ptr @vec_at(ptr noundef %69, i32 noundef %70)
  %72 = call ptr @copy_node(ptr noundef %71)
  call void @vec_push_last(ptr noundef %66, ptr noundef %72)
  br label %73

73:                                               ; preds = %63
  %74 = load i32, ptr %5, align 4
  %75 = add nsw i32 %74, 1
  store i32 %75, ptr %5, align 4
  br label %55, !llvm.loop !17

76:                                               ; preds = %55
  br label %77

77:                                               ; preds = %76, %10
  %78 = load ptr, ptr %3, align 8
  %79 = getelementptr inbounds %struct.Node, ptr %78, i32 0, i32 6
  %80 = load ptr, ptr %79, align 8
  %81 = icmp ne ptr %80, null
  br i1 %81, label %82, label %108

82:                                               ; preds = %77
  %83 = call ptr @new_vector()
  %84 = load ptr, ptr %4, align 8
  %85 = getelementptr inbounds %struct.Node, ptr %84, i32 0, i32 6
  store ptr %83, ptr %85, align 8
  store i32 0, ptr %6, align 4
  br label %86

86:                                               ; preds = %104, %82
  %87 = load i32, ptr %6, align 4
  %88 = load ptr, ptr %3, align 8
  %89 = getelementptr inbounds %struct.Node, ptr %88, i32 0, i32 6
  %90 = load ptr, ptr %89, align 8
  %91 = getelementptr inbounds %struct.vector, ptr %90, i32 0, i32 1
  %92 = load i32, ptr %91, align 8
  %93 = icmp slt i32 %87, %92
  br i1 %93, label %94, label %107

94:                                               ; preds = %86
  %95 = load ptr, ptr %4, align 8
  %96 = getelementptr inbounds %struct.Node, ptr %95, i32 0, i32 6
  %97 = load ptr, ptr %96, align 8
  %98 = load ptr, ptr %3, align 8
  %99 = getelementptr inbounds %struct.Node, ptr %98, i32 0, i32 6
  %100 = load ptr, ptr %99, align 8
  %101 = load i32, ptr %6, align 4
  %102 = call ptr @vec_at(ptr noundef %100, i32 noundef %101)
  %103 = call ptr @copy_lvar(ptr noundef %102)
  call void @vec_push_last(ptr noundef %97, ptr noundef %103)
  br label %104

104:                                              ; preds = %94
  %105 = load i32, ptr %6, align 4
  %106 = add nsw i32 %105, 1
  store i32 %106, ptr %6, align 4
  br label %86, !llvm.loop !18

107:                                              ; preds = %86
  br label %108

108:                                              ; preds = %107, %77
  %109 = load ptr, ptr %3, align 8
  %110 = getelementptr inbounds %struct.Node, ptr %109, i32 0, i32 7
  %111 = load ptr, ptr %110, align 8
  %112 = call ptr @copy_var(ptr noundef %111)
  %113 = load ptr, ptr %4, align 8
  %114 = getelementptr inbounds %struct.Node, ptr %113, i32 0, i32 7
  store ptr %112, ptr %114, align 8
  %115 = load ptr, ptr %3, align 8
  %116 = getelementptr inbounds %struct.Node, ptr %115, i32 0, i32 8
  %117 = load i32, ptr %116, align 8
  %118 = load ptr, ptr %4, align 8
  %119 = getelementptr inbounds %struct.Node, ptr %118, i32 0, i32 8
  store i32 %117, ptr %119, align 8
  %120 = load ptr, ptr %3, align 8
  %121 = getelementptr inbounds %struct.Node, ptr %120, i32 0, i32 9
  %122 = load i32, ptr %121, align 4
  %123 = load ptr, ptr %4, align 8
  %124 = getelementptr inbounds %struct.Node, ptr %123, i32 0, i32 9
  store i32 %122, ptr %124, align 4
  %125 = load ptr, ptr %4, align 8
  store ptr %125, ptr %2, align 8
  br label %126

126:                                              ; preds = %108, %9
  %127 = load ptr, ptr %2, align 8
  ret ptr %127
}

declare ptr @copy_lvar(ptr noundef) #2

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind allocsize(0,1) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #5 = { nounwind allocsize(0,1) }
attributes #6 = { nounwind }

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
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
!13 = distinct !{!13, !7}
!14 = distinct !{!14, !7}
!15 = distinct !{!15, !7}
!16 = distinct !{!16, !7}
!17 = distinct !{!17, !7}
!18 = distinct !{!18, !7}
