CFLAGS=-std=c11 -g -static -Werror -Wall -Wextra
SRCS=$(notdir $(wildcard src/*.c))
OBJS=$(addprefix dist/,$(notdir $(SRCS:.c=.o)))
TARGET=dist/1cc

1cc: $(TARGET)

$(TARGET): $(OBJS)
	clang $(CFLAGS) -o $@ $^

dist/%.o: src/%.c
	clang $(CFLAGS) -c -o $@ $<

test: $(TARGET)
	./test.sh

single-test: $(TARGET)
	./test.sh $(TEST)

clean:
	rm -f $(TARGET) dist/*.o dist/*~ dist/tmp*

.PHONY: test clean
