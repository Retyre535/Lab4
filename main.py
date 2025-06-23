import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os

def visualize_performance():
    file_path = 'performance_results.csv'
    if not os.path.exists(file_path):
        print(f"Error: {file_path} not found.")
        print("Please run the performance tests in your C++ program first.")
        return

    try:
        df = pd.read_csv(file_path)

        grouped = df.groupby(['operation', 'size'])['time_ms'].mean().reset_index()

        operations = df['operation'].unique()
        sizes = sorted(df['size'].unique())

        fig, axes = plt.subplots(2, 3, figsize=(18, 12))
        axes = axes.flatten()

        for i, operation in enumerate(operations):
            if i >= len(axes):
                break

            op_data = grouped[grouped['operation'] == operation]
            axes[i].plot(op_data['size'], op_data['time_ms'], 'o-', linewidth=2, markersize=8)
            axes[i].set_title(f'{operation.capitalize()} Operation', fontsize=14)
            axes[i].set_xlabel('Number of Elements', fontsize=12)
            axes[i].set_ylabel('Time (ms)', fontsize=12)
            axes[i].grid(True)
            axes[i].set_xscale('log')

        for j in range(i+1, len(axes)):
            axes[j].set_visible(False)

        plt.tight_layout()
        plt.suptitle('Binary Tree Performance Analysis', fontsize=16)
        plt.subplots_adjust(top=0.92)

        plt.savefig('binary_tree_performance.png', dpi=300, bbox_inches='tight')

        plt.show()

        plt.figure(figsize=(12, 8))

        common_sizes = []
        for operation in operations:
            op_data = grouped[grouped['operation'] == operation]
            if common_sizes:
                common_sizes = list(set(common_sizes) & set(op_data['size']))
            else:
                common_sizes = list(op_data['size'])

        common_sizes.sort()

        if common_sizes:
            width = 0.2
            positions = np.arange(len(common_sizes))

            for i, operation in enumerate(operations):
                op_data = grouped[grouped['operation'] == operation]
                op_data = op_data[op_data['size'].isin(common_sizes)]

                if not op_data.empty:
                    offset = (i - len(operations)/2 + 0.5) * width
                    plt.bar(positions + offset, op_data['time_ms'], width, label=operation.capitalize())

            plt.xlabel('Number of Elements', fontsize=14)
            plt.ylabel('Time (ms)', fontsize=14)
            plt.title('Performance Comparison Across Operations', fontsize=16)
            plt.xticks(positions, common_sizes)
            plt.legend()
            plt.grid(True, axis='y')
            plt.tight_layout()

            plt.savefig('binary_tree_performance_comparison.png', dpi=300, bbox_inches='tight')

            plt.show()

        print(f"Visualizations have been saved as 'binary_tree_performance.png' and 'binary_tree_performance_comparison.png'")

    except Exception as e:
        print(f"Error visualizing performance data: {e}")

if __name__ == "__main__":
    visualize_performance()
