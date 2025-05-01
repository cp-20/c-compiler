CFLAGS=-std=c11 -g -static -Werror -Wall -Wextra
SRCS=$(notdir $(wildcard src/*.c))
LIBS=$(notdir $(wildcard src/lib/*.c))
LIBOBJS=$(addprefix dist/lib/,$(notdir $(LIBS:.c=.o)))
OBJS_1CC=$(addprefix dist/1cc/,$(notdir $(SRCS:.c=.o)))
OBJS_2CC=$(addprefix dist/2cc/,$(notdir $(SRCS:.c=.o)))
OBJS_3CC=$(addprefix dist/3cc/,$(notdir $(SRCS:.c=.o)))
TARGET_1CC=dist/1cc/compiler
TARGET_2CC=dist/2cc/compiler
TARGET_3CC=dist/3cc/compiler

1cc: $(TARGET_1CC)
2cc: $(TARGET_2CC)
3cc: $(TARGET_3CC)

$(TARGET_1CC): $(OBJS_1CC) $(LIBOBJS)
	clang $(CFLAGS) -o $@ $(OBJS_1CC)
$(TARGET_2CC): $(OBJS_2CC) $(LIBOBJS)
	clang $(CFLAGS) -o $@ $(OBJS_2CC)
$(TARGET_3CC): $(OBJS_3CC) $(LIBOBJS)
	clang $(CFLAGS) -o $@ $(OBJS_3CC)

dist/1cc/%.o: src/%.c | dist/1cc
	clang $(CFLAGS) -c -o $@ $<

dist/2cc/%.o: dist/2cc/%.ll | dist/2cc
	clang $(CFLAGS) -c -o $@ $<
dist/2cc/%.ll: src/%.c $(TARGET_1CC) src/lib.ll | dist/2cc
	./$(TARGET_1CC) --output $@ --suppress $<

dist/3cc/%.o: dist/3cc/%.ll | dist/3cc
	clang $(CFLAGS) -c -o $@ $<
dist/3cc/%.ll: src/%.c $(TARGET_2CC) src/lib.ll | dist/3cc
	./$(TARGET_2CC) --output $@ --suppress $<

dist/lib/%.o: src/lib/%.c | dist/lib
	clang $(CFLAGS) -c -o $@ $<

dist:
	mkdir -p dist

dist/lib:
	mkdir -p dist/lib

dist/1cc:
	mkdir -p dist/1cc
dist/2cc:
	mkdir -p dist/2cc
dist/3cc:
	mkdir -p dist/3cc

test-1cc: $(TARGET_1CC)
	./test.sh --compiler $(TARGET_1CC)
test-2cc: $(TARGET_2CC)
	./test.sh --compiler $(TARGET_2CC)
test-3cc: $(TARGET_3CC)
	./test.sh --compiler $(TARGET_3CC)

perf-1cc: $(TARGET_1CC)
	./perf.sh --compiler $(TARGET_1CC)
perf-2cc: $(TARGET_2CC)
	./perf.sh --compiler $(TARGET_2CC)

test-1cc-ok: $(TARGET_1CC)
	./test.sh --show-ok-result --compiler $(TARGET_1CC)
test-2cc-ok: $(TARGET_2CC)
	./test.sh --show-ok-result --compiler $(TARGET_2CC)
test-3cc-ok: $(TARGET_3CC)
	./test.sh --show-ok-result --compiler $(TARGET_3CC)

diff-2cc-3cc: $(TARGET_2CC) $(TARGET_3CC)
	ls dist/2cc/*.ll | xargs -I{} sh -c 'diff {} dist/3cc/`basename {}`'

clean:
	rm -f dist/lib/*.o \
		$(TARGET_1CC) dist/1cc/*.o dist/1cc/*~ \
		$(TARGET_2CC) dist/2cc/*.o dist/2cc/*.ll dist/2cc/*~ \
		$(TARGET_3CC) dist/3cc/*.o dist/3cc/*.ll dist/3cc/*~

.PHONY: test clean

.PRECIOUS: dist/2cc/%.ll
.PRECIOUS: dist/3cc/%.ll
