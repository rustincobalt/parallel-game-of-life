import pandas as pd
import matplotlib.pyplot as plt

# =========================================================
# FILES
# =========================================================

SEQ_FILE = "./data/SeqPure.csv"
OMP_FILE = "./data/RowPure.csv"

# =========================================================
# LOAD DATA
# =========================================================

seq = pd.read_csv(SEQ_FILE)
omp = pd.read_csv(OMP_FILE)

# Keep only AVG rows
seq = seq[seq["Run"] == "AVG"].copy()
omp = omp[omp["Run"] == "AVG"].copy()

# Convert numeric columns
numeric_cols = [
    "Width",
    "Height",
    "Threads",
    "TimeMs"
]

for col in numeric_cols:
    if col in seq.columns:
        seq[col] = pd.to_numeric(seq[col])

    if col in omp.columns:
        omp[col] = pd.to_numeric(omp[col])





# # Divide all measured times by 250
seq["TimeMs"] = seq["TimeMs"] / 250.0
omp["TimeMs"] = omp["TimeMs"] / 250.0

# =========================================================
# MERGE SEQUENTIAL + PARALLEL
# =========================================================

merged = pd.merge(
    omp,
    seq[[
        "Width",
        "Height",
        "Seed",
        "FillPercent",
        "TimeMs"
    ]],
    on=[
        "Width",
        "Height",
        "Seed",
        "FillPercent"
    ],
    suffixes=("_omp", "_seq")
)


# =========================================================
# CALCULATE SPEEDUP + EFFICIENCY
# =========================================================

merged["Speedup"] = (
    merged["TimeMs_seq"] /
    merged["TimeMs_omp"]
)

merged["Efficiency"] = (
    merged["Speedup"] /
    merged["Threads"]
)

# Grid size label
merged["SizeLabel"] = (
    merged["Width"].astype(str)
    + "x" +
    merged["Height"].astype(str)
)

# Numeric total size
merged["TotalCells"] = (
    merged["Width"] *
    merged["Height"]
)

# =========================================================
# 1. TIME VS SIZE
# =========================================================

def plot_time_vs_size():

    plt.figure(figsize=(12, 7))

    # =====================================================
    # Sequential
    # =====================================================

    seq_plot = seq.copy()

    seq_plot["TotalCells"] = (
        seq_plot["Width"] *
        seq_plot["Height"]
    )

    seq_plot["SizeLabel"] = (
        seq_plot["Width"].astype(str)
        + "x" +
        seq_plot["Height"].astype(str)
    )

    seq_plot = seq_plot.sort_values("TotalCells")

    plt.plot(
        seq_plot["TotalCells"],
        seq_plot["TimeMs"],
        marker="o",
        linewidth=3,
        label="Sequential"
    )

    # =====================================================
    # Parallel versions
    # =====================================================

    for threads in sorted(omp["Threads"].unique()):

        omp_plot = omp[
            omp["Threads"] == threads
        ].copy()

        omp_plot["TotalCells"] = (
            omp_plot["Width"] *
            omp_plot["Height"]
        )

        omp_plot = omp_plot.sort_values("TotalCells")

        plt.plot(
            omp_plot["TotalCells"],
            omp_plot["TimeMs"],
            marker="o",
            linewidth=2,
            label=f"OMP ({threads} Threads)"
        )

    # =====================================================
    # Custom X-axis labels
    # =====================================================

    x_ticks = seq_plot["TotalCells"]
    x_labels = seq_plot["SizeLabel"]

    plt.xticks(
        ticks=x_ticks,
        labels=x_labels,
        rotation=45
    )

    # =====================================================
    # Styling
    # =====================================================

    plt.title("Execution Time vs Grid Size")
    plt.xlabel("Grid Size")
    plt.ylabel("Average Time (ms)")

    plt.grid(True, linestyle="--", alpha=0.6)

    plt.legend()

    plt.tight_layout()

    plt.show()
# =========================================================
# 2. SPEEDUP VS THREADS
# =========================================================

def plot_speedup_vs_threads():

    plt.figure(figsize=(10, 6))

    for size in merged["SizeLabel"].unique():

        subset = merged[
            merged["SizeLabel"] == size
        ].copy()

        subset = subset.sort_values("Threads")

        plt.plot(
            subset["Threads"],
            subset["Speedup"],
            marker="o",
            linewidth=2,
            label=size
        )

    # Ideal speedup line
    max_threads = merged["Threads"].max()

    plt.plot(
        range(1, max_threads + 1),
        range(1, max_threads + 1),
        linestyle="--",
        label="Ideal Speedup"
    )

    plt.title("Speedup vs Threads")
    plt.xlabel("Threads")
    plt.ylabel("Speedup")

    plt.grid(True, linestyle="--", alpha=0.6)
    plt.legend(title="Grid Size")

    plt.tight_layout()

    plt.show()

# =========================================================
# 3. EFFICIENCY VS THREADS
# =========================================================

def plot_efficiency_vs_threads():

    plt.figure(figsize=(10, 6))

    for size in merged["SizeLabel"].unique():

        subset = merged[
            merged["SizeLabel"] == size
        ].copy()

        subset = subset.sort_values("Threads")

        plt.plot(
            subset["Threads"],
            subset["Efficiency"],
            marker="o",
            linewidth=2,
            label=size
        )

    plt.title("Efficiency vs Threads")
    plt.xlabel("Threads")
    plt.ylabel("Efficiency")

    plt.grid(True, linestyle="--", alpha=0.6)
    plt.legend(title="Grid Size")

    plt.tight_layout()

    plt.show()


# =========================================================
# SHOW SPEEDUP + TIME FOR 16 THREADS
# =========================================================

threads_to_show = 16

subset = merged[
    merged["Threads"] == threads_to_show
].copy()

subset = subset.sort_values("TotalCells")

print(f"\n===== {threads_to_show} THREADS =====\n")

for _, row in subset.iterrows():

    size = f"{int(row['Width'])}x{int(row['Height'])}"

    speedup = row["Speedup"]

    # original measured time (not divided by 250)
    total_time = row["TimeMs_omp"]

    print(
        f"{size} | "
        f"Speedup: {speedup:.2f} | "
        f"Total Time: {total_time:.3f} ms"
    )


# =========================================================
# EXAMPLES
# =========================================================


def plot_time_16_vs_sequential():

    threads_to_show = 16

    plt.figure(figsize=(10, 6))

    # =====================================================
    # Sequential
    # =====================================================

    seq_plot = seq.copy()

    seq_plot["TotalCells"] = (
        seq_plot["Width"] *
        seq_plot["Height"]
    )

    seq_plot["SizeLabel"] = (
        seq_plot["Width"].astype(str)
        + "x" +
        seq_plot["Height"].astype(str)
    )

    seq_plot = seq_plot.sort_values("TotalCells")

    plt.plot(
        seq_plot["SizeLabel"],
        seq_plot["TimeMs"],
        marker="o",
        linewidth=3,
        label="Sequential"
    )

    # =====================================================
    # 16 Threads
    # =====================================================

    omp_plot = omp[
        omp["Threads"] == threads_to_show
    ].copy()

    omp_plot["TotalCells"] = (
        omp_plot["Width"] *
        omp_plot["Height"]
    )

    omp_plot["SizeLabel"] = (
        omp_plot["Width"].astype(str)
        + "x" +
        omp_plot["Height"].astype(str)
    )

    omp_plot = omp_plot.sort_values("TotalCells")

    plt.plot(
        omp_plot["SizeLabel"],
        omp_plot["TimeMs"],
        marker="o",
        linewidth=3,
        label=f"OMP ({threads_to_show} Threads)"
    )

    # =====================================================
    # Styling
    # =====================================================

    plt.title(f"Execution Time: Sequential vs {threads_to_show} Threads")
    plt.xlabel("Grid Size")
    plt.ylabel("Time (ms)")

    plt.grid(True, linestyle="--", alpha=0.6)

    plt.legend()

    plt.tight_layout()

    plt.show()


def plot_speedup_16_threads():

    threads_to_show = 16

    subset = merged[
        merged["Threads"] == threads_to_show
    ].copy()

    subset = subset.sort_values("TotalCells")

    plt.figure(figsize=(10, 6))

    plt.plot(
        subset["SizeLabel"],
        subset["Speedup"],
        marker="o",
        linewidth=2,
        label=f"{threads_to_show} Threads"
    )

    plt.title(f"Speedup for {threads_to_show} Threads")
    plt.xlabel("Grid Size")
    plt.ylabel("Speedup")

    plt.grid(True, linestyle="--", alpha=0.6)

    plt.legend()

    plt.tight_layout()

    plt.show()


# Time comparison for selected thread count
# plot_time_vs_size()
# plot_time_vs_size(4)
# plot_time_vs_size(8)

# Speedup graph
# plot_speedup_vs_threads()
# plot_speedup_16_threads()

# plot_time_16_vs_sequential()

# Efficiency graph
plot_efficiency_vs_threads()

# =========================================================
# SPEEDUP TABLE
# Rows    -> Grid sizes
# Columns -> Thread counts
# Values  -> Speedup
# =========================================================

# Create pivot table
speedup_table = merged.pivot_table(
    index="SizeLabel",
    columns="Threads",
    values="Speedup"
)

# Sort rows by actual grid size
size_order = (
    merged[["SizeLabel", "TotalCells"]]
    .drop_duplicates()
    .sort_values("TotalCells")
)

speedup_table = speedup_table.reindex(
    size_order["SizeLabel"]
)

# Round values
speedup_table = speedup_table.round(2)

# Display
print("\n===== SPEEDUP TABLE =====\n")
print(speedup_table.to_string())


# =========================================================
# EFFICIENCY TABLE
# Rows    -> Grid sizes
# Columns -> Thread counts
# Values  -> Efficiency
# =========================================================

# Create pivot table
efficiency_table = merged.pivot_table(
    index="SizeLabel",
    columns="Threads",
    values="Efficiency"
)

# Sort rows by actual grid size
size_order = (
    merged[["SizeLabel", "TotalCells"]]
    .drop_duplicates()
    .sort_values("TotalCells")
)

efficiency_table = efficiency_table.reindex(
    size_order["SizeLabel"]
)

# Round values
efficiency_table = efficiency_table.round(3)

# Display
print("\n===== EFFICIENCY TABLE =====\n")
print(efficiency_table.to_string())