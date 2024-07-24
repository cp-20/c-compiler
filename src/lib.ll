; My library
declare i32 @print(i32 noundef, ...) #0
declare i32 @scan() #0
declare i32 @alloc4(i32** noundef, i32 noundef, i32 noundef, i32 noundef, i32 noundef) #0

; stdlib.h
declare noalias ptr @calloc(i64 noundef, i64 noundef) #0
declare noalias ptr @malloc(i64 noundef) #0
declare ptr @realloc(ptr noundef, i64 noundef) #0
declare void @free(ptr noundef) #0
declare void @exit(i32 noundef) #0

; stdarg.h
declare void @llvm.va_start(ptr) #0
declare void @llvm.va_end(ptr) #0

; stdio.h
declare i32 @printf(ptr noundef, ...) #0
declare i32 @fprintf(ptr noundef, ptr noundef, ...) #0
declare i32 @vsnprintf(ptr noundef, i64 noundef, ptr noundef, ptr noundef) #0
declare i32 @sprintf(ptr noundef, ptr noundef, ...) #0
declare i32 @vprintf(ptr noundef, ptr noundef) #2
