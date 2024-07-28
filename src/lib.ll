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
declare i32 @memcmp(ptr noundef, ptr noundef, i64 noundef) #0
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #0

; stdarg.h
declare void @llvm.va_start(ptr) #0
declare void @llvm.va_end(ptr) #0

; stdio.h
declare i32 @printf(ptr noundef, ...) #0
declare i32 @fprintf(ptr noundef, ptr noundef, ...) #0
declare i32 @vsnprintf(ptr noundef, i64 noundef, ptr noundef, ptr noundef) #0
declare i32 @sprintf(ptr noundef, ptr noundef, ...) #0
declare i32 @vprintf(ptr noundef, ptr noundef) #0
declare i32 @vfprintf(ptr noundef, ptr noundef, ptr noundef) #0
declare i32 @snprintf(ptr noundef, i64 noundef, ptr noundef, ...) #0
declare noalias ptr @fopen(ptr noundef, ptr noundef) #0
declare i32 @fclose(ptr noundef) #0
declare i64 @ftell(ptr noundef) #0
declare i64 @fread(ptr noundef, i64 noundef, i64 noundef, ptr noundef) #0
declare i32 @fseek(ptr noundef, i64 noundef, i32 noundef) #0

; string.h
declare ptr @strcat(ptr noundef, ptr noundef) #0
declare i32 @strcmp(ptr noundef, ptr noundef) #0
declare i64 @strlen(ptr noundef) #0
declare ptr @strncat(ptr noundef, ptr noundef, i64 noundef) #0
declare i32 @strncmp(ptr noundef, ptr noundef, i64 noundef) #0
declare ptr @strerror(i32 noundef) #0
declare ptr @strstr(ptr noundef, ptr noundef) #0
declare i64 @strtol(ptr noundef, ptr noundef, i32 noundef) #0
declare ptr @strncpy(ptr noundef, ptr noundef, i64 noundef) #0

; errno.h
declare ptr @__errno_location() #0
