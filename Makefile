CFLAGS=-std=c11 -g -static -Werror -Wall -Wextra
SRCS=$(notdir $(wildcard src/*.c))
OBJS=$(addprefix dist/,$(notdir $(SRCS:.c=.o)))
OBJS_SELF=$(addprefix dist/compiler/,$(notdir $(SRCS:.c=.o)))
LIBS=$(notdir $(wildcard src/lib/*.c))
LIBOBJS=$(addprefix dist/lib/,$(notdir $(LIBS:.c=.o)))
TARGET=dist/1cc
TARGET_SELF=dist/compiler/1cc

1cc: $(TARGET)

self-1cc: $(TARGET_SELF)

$(TARGET): $(OBJS) $(LIBOBJS)
	clang $(CFLAGS) -o $@ $(OBJS)

$(TARGET_SELF): $(OBJS_SELF) $(LIBOBJS)
	clang $(CFLAGS) -o $@ $(OBJS_SELF)

dist/%.o: src/%.c | dist
	clang $(CFLAGS) -c -o $@ $<

dist/lib/%.o: src/lib/%.c | dist/lib
	clang $(CFLAGS) -c -o $@ $<

dist/compiler/%.o: dist/compiler/%.ll $(TARGET) | dist/compiler
	clang $(CFLAGS) -c -o $@ $<

dist/compiler/%.ll: src/%.c $(TARGET) src/lib.ll | dist/compiler
	./$(TARGET) --output $@ $<

.PRECIOUS: dist/compiler/%.ll

dist:
	mkdir -p dist

dist/lib:
	mkdir -p dist/lib

dist/compiler:
	mkdir -p dist/compiler

test: $(TARGET)
	./test.sh

test-self: $(TARGET_SELF)
	./test.sh --self

test-ok: $(TARGET)
	./test.sh --show-ok-result

clean:
	rm -f $(TARGET) dist/*.o dist/*~ dist/lib/*.o dist/compiler/*.o dist/compiler/*.ll

.PHONY: test clean
