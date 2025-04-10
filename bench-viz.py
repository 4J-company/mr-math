import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D

# Load the CSV data
df = pd.read_csv('bench-output.csv')

# Function to parse benchmark names into components
def parse_benchmark_name(name):
    base = name.split('<')[0]
    params = name.split('<')[1].split('>')[0].split(',') if '<' in name else []
    params = [p.strip() for p in params]
    dtype = params[0] if len(params) > 0 else None
    size = int(params[1]) if len(params) > 1 else None
    return base, dtype, size

# Add parsed components to DataFrame
df[['benchmark', 'type', 'size']] = df['name'].apply(
    lambda x: pd.Series(parse_benchmark_name(x)))
df['size'] = df['size'].astype(pd.Int64Dtype())  # Handle NaN sizes

# Filter benchmarks with size information
sized_benchmarks = df[df['size'].notna()]

# Create 3D plots for each benchmark that has size parameter
for bench_name in sized_benchmarks['benchmark'].unique():
    bench_group = sized_benchmarks[sized_benchmarks['benchmark'] == bench_name]

    # Create pivot table for 3D plotting
    pivot_table = bench_group.pivot_table(
        index='size',
        columns='type',
        values='cpu_time',
        aggfunc='mean'
    ).fillna(0)

    types = pivot_table.columns.tolist()
    sizes = pivot_table.index.tolist()

    x = np.arange(len(types))
    y = np.arange(len(sizes))
    X, Y = np.meshgrid(x, y)
    Z = pivot_table.values

    # Create 3D plot
    fig = plt.figure(figsize=(12, 8))
    ax = fig.add_subplot(111, projection='3d')

    # Plot surface
    surf = ax.plot_surface(X, Y, Z, cmap='viridis', edgecolor='k')

    # Customize axes
    ax.set_xticks(x)
    ax.set_xticklabels(types)
    ax.set_yticks(y)
    ax.set_yticklabels(sizes)
    ax.set_xlabel('Data Type')
    ax.set_ylabel('Matrix/Vector Size')
    ax.set_zlabel('CPU Time (ns)')

    plt.title(f'{bench_name} Performance Characteristics')
    fig.colorbar(surf, shrink=0.5, aspect=5)
    plt.tight_layout()
    plt.show()

# Additional 2D plots for benchmarks without size parameter
non_typed = df[df['type'].isna()]
non_sized = df[df['size'].isna()]

for bench_name in non_sized['benchmark'].unique():
    if bench_name in non_typed['benchmark'].unique():
        continue

    bench_group = non_sized[non_sized['benchmark'] == bench_name]
    plt.figure(figsize=(10, 6))
    plt.bar(bench_group['type'], bench_group['cpu_time'])
    plt.xlabel('Data Type')
    plt.ylabel('CPU Time (ns)')
    plt.title(f'{bench_name} Performance Comparison')
    plt.show()

plt.figure(figsize=(15, 8))
bars = plt.bar(non_typed['benchmark'], non_typed['cpu_time'])
plt.xticks(rotation=45, ha='right', fontsize=8)
plt.xlabel('Benchmark Name')
plt.ylabel('CPU Time (ns)')
plt.title('Performance of Non-Templated Benchmarks')

# Add value labels on top of bars
for bar in bars:
    height = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2., height,
             f'{height:.2f}',
             ha='center', va='bottom', rotation=45, fontsize=8)

plt.tight_layout()
plt.show()
