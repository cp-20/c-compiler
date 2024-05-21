CFLAGS=-std=c11 -g -static -Werror -Wall -Wextra
SRCS=$(wildcard *.c)
OBJS=$(addprefix dist/,$(notdir $(SRCS:.c=.o)))
TARGET=dist/1cc

$(TARGET): $(OBJS)
	clang $(CFLAGS) -o $@ $^

dist/%.o: %.c
	clang $(CFLAGS) -c -o $@ $<

test: $(TARGET)
	./test.sh

single-test: $(TARGET)
	./test.sh $(TEST)

clean:
	rm -f $(TARGET) dist/*.o dist/*~ dist/tmp*

.PHONY: test clean
