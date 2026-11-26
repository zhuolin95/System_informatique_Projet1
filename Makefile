
CC = gcc
CFLAGS = -O0 -Wall -Wextra -pthread

PART1_SRCS = src/part1/philosophes.c \
             src/part1/producer_consumer.c \
             src/part1/reader_writer.c

PART2_SRCS =

SRCS = $(PART1_SRCS) $(PART2_SRCS)
TARGETS = $(SRCS:.c=)

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

test: all
	chmod +x experiments.sh src/part1/benchmark_part1.sh
	./experiments.sh

clean:
	rm -f $(TARGETS)
	rm -rf results/

.PHONY: all test clean
