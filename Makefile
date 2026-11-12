CC = cc
CFLAGS = -Wall -Wextra -O2 -std=c11
LDFLAGS =
TARGET = test_memory_pool
SRCS = tests/test_memory_pool.c src/memory_pool.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	./$(TARGET)