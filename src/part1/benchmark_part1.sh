cd "$(dirname "$0")"
mkdir -p ../../results
OUTPUT="../../results/philosophes.csv"

echo "threads,run,time" > "$OUTPUT"

for threads in 2 4 8 16 32; do
    for run in 1 2 3 4 5; do
        exec_time=$(/usr/bin/time -f "%e" ./philosophes $threads 2>&1 >/dev/null)
        echo "$threads,$run,$exec_time" >> "$OUTPUT"
    done
donegit 