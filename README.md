CC = gcc
CFLAGS = -O0 -Wall -Wextra -pthread

PART1_SRCS = src/part1/philosophes.c

SRCS = $(PART1_SRCS) $(PART2_SRCS)
TARGETS = $(SRCS:.c=)

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

test: all
	chmod +x experiments.sh src/part1/*.sh src/part2/*.sh src/part2/tas/*.sh src/part2/tatas/*.sh 2>/dev/null || true
	./experiments.sh

clean:
	rm -f $(TARGETS)
	rm -rf results/

.PHONY: all test clean