CFLAGS=-std=c11 -g -static -Werror -Weverything -Wno-declaration-after-statement -Wno-strict-prototypes -Wno-unsafe-buffer-usage -Wno-padded
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
