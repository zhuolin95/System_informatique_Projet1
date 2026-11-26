#!/bin/bash

set -e

echo "=== Partie 1 : Benchmarks ==="

bash src/part1/benchmark_part1.sh

echo ""
echo "======================================"
echo "       RESULTATS DES BENCHMARKS"
echo "======================================"

for file in results/*.csv; do
    echo ""
    echo "=== $file ==="
    cat "$file"
done

echo ""
echo "=== Expériences terminées ==="
