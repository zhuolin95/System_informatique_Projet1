import csv
import math
import os
import matplotlib.pyplot as plt

def load_and_compute_stats(csv_path):
    data = {}
    with open(csv_path, mode='r', encoding='utf-8') as f:
        reader = csv.DictReader(f)
        for row in reader:
            threads = int(row['threads'])
            time_sec = float(row['time_seconds'])
            data.setdefault(threads, []).append(time_sec)

    sorted_threads = sorted(data.keys())
    means = []
    stds = []

    for t in sorted_threads:
        times = data[t]
        n = len(times)
        mean_val = sum(times) / n
        if n > 1:
            variance = sum((x - mean_val) ** 2 for x in times) / (n - 1)
            std_val = math.sqrt(variance)
        else:
            std_val = 0.0
        means.append(mean_val)
        stds.append(std_val)

    return sorted_threads, means, stds

def plot_benchmark(csv_file, output_pdf, title, label_name):
    if not os.path.exists(csv_file):
        print("Execute ~bash experiments.sh~ before generating plot")
        return

    threads, means, stds = load_and_compute_stats(csv_file)

    plt.figure(figsize=(8, 5))

    plt.errorbar(
        threads, means, yerr=stds,
        fmt='-o', capsize=8, capthick=1.5, elinewidth=1.5,
        markersize=4,
        markerfacecolor='#d62728',
        markeredgecolor='#d62728',
        linewidth=1.5,
        label=label_name
    )


    plt.xlabel("Nombre total de threads", fontsize=12)
    plt.ylabel("Temps d'exécution (s)", fontsize=12)
    plt.title(title, fontsize=13, fontweight='bold')


    plt.ylim(bottom=0)
    

    plt.xticks(threads)
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.legend(fontsize=11)
    plt.tight_layout()


    plt.savefig(output_pdf, format='pdf')
    plt.close()


if __name__ == "__main__":
    os.makedirs("results", exist_ok=True)


    plot_benchmark(
        csv_file="results/philosophes.csv",
        output_pdf="results/philosophes_part1.pdf",
        title="philosophes ",
        label_name ="POSIX"
    )

    plot_benchmark(
        csv_file="results/producer_consumer.csv",
        output_pdf="results/producer_consumer_part1.pdf",
        title="Producteurs-Consommateurs",
        label_name ="POSIX"
    )

    plot_benchmark(
        csv_file="results/reader_writer.csv",
        output_pdf="results/reader_writer_part1.pdf",
        title="Lecteurs-Écrivains",
        label_name ="POSIX"
    )