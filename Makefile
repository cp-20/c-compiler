CFLAGS=-std=c11 -g -static -Werror -Wall -Wextra
SRCS=$(notdir $(wildcard src/*.c))
OBJS=$(addprefix dist/,$(notdir $(SRCS:.c=.o)))
LIBS=$(notdir $(wildcard src/lib/*.c))
LIBOBJS=$(addprefix dist/lib/,$(notdir $(LIBS:.c=.o)))
TARGET=dist/1cc

1cc: $(TARGET)

$(TARGET): $(OBJS) $(LIBOBJS)
	clang $(CFLAGS) -o $@ $^

dist/%.o: src/%.c dist
	clang $(CFLAGS) -c -o $@ $<

dist/lib/%.o: src/lib/%.c dist/lib
	clang $(CFLAGS) -c -o $@ $<

dist:
	mkdir -p dist

dist/lib:
	mkdir -p dist/lib

test: $(TARGET)
	./test.sh

single-test: $(TARGET)
	./test.sh $(TEST)

clean:
	rm -f $(TARGET) dist/*.o dist/*~ dist/lib/*.o

.PHONY: test clean
