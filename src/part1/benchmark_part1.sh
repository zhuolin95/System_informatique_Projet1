#!/bin/bash

mkdir -p results

echo "threads,run,time_seconds" > results/philosophes.csv
echo "threads,run,time_seconds" > results/producer_consumer.csv
echo "threads,run,time_seconds" > results/reader_writer.csv
THREADS=(2 4 8 16 32)
REPETITIONS=5

measure_time() {
    local start end

    start=$(date +%s%N)

    "$@" > /dev/null 2>&1

    end=$(date +%s%N)

    echo "scale=9; ($end - $start) / 1000000000" | bc
}

# Benchmark philosophe
for n in "${THREADS[@]}"; do
    for run in $(seq 1 "$REPETITIONS"); do

        time=$(measure_time ./src/part1/philosophes "$n")

        echo "$n,$run,$time" >> results/philosophes.csv
    done
done

# Benchmark producer
for n in "${THREADS[@]}"; do
    half=$((n / 2))

    for run in $(seq 1 "$REPETITIONS"); do

        time=$(measure_time ./src/part1/producer_consumer "$half" "$half")

        echo "$n,$run,$time" >> results/producer_consumer.csv
    done
done

# Benchmark ecrivain
for n in "${THREADS[@]}"; do
    half=$((n / 2))

    for run in $(seq 1 "$REPETITIONS"); do

        time=$(measure_time ./src/part1/reader_writer "$half" "$half")

        echo "$n,$run,$time" >> results/reader_writer.csv
    done
done

echo "Benchmarks terminés."
