import os
import pandas as pd
import matplotlib.pyplot as plt

def plot_simulation_results(folder_path):
    csv_files = [f for f in os.listdir(folder_path) if f.endswith('.csv')]
    
    if not csv_files:
        print(f"No CSV files found in the directory: {folder_path}")
        return

    fig, ax = plt.subplots(figsize=(12, 8))
    
    line_styles = ['-', '--', '-.', ':']
    num_line_styles = len(line_styles)

    data_dict = {}

    for csv_file in csv_files:
        file_path = os.path.join(folder_path, csv_file)
        df = pd.read_csv(file_path)

        k_value = int(csv_file.split('x')[1].split('.')[0])

        data_dict[k_value] = df

    sorted_k_values = sorted(data_dict.keys())

    for i, k_value in enumerate(sorted_k_values):
        df = data_dict[k_value]
        line_style = line_styles[i % num_line_styles]
        ax.plot(df['sigma'], df['error_rate'], linestyle=line_style, label=f'20x{k_value}')
    
    ax.set_title('Error Rate vs Sigma')
    ax.set_xlabel('Sigma')
    ax.set_ylabel('Error Rate')
    ax.legend(title='Codeword Size', bbox_to_anchor=(1.05, 1), loc='upper left')
    ax.grid(True)

    output_file = os.path.join(folder_path, 'combined_plot.png')
    plt.savefig(output_file, bbox_inches='tight')
    print(f"Plot saved to {output_file}")

    plt.show()

if __name__ == "__main__":
    import sys
    
    if len(sys.argv) != 2:
        print("Usage: python visualize.py <path_to_folder>")
        sys.exit(1)
    
    folder_path = sys.argv[1]
    plot_simulation_results(folder_path)