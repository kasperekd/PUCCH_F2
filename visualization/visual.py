import os
import pandas as pd
import matplotlib.pyplot as plt

def plot_simulation_results(folder_path):
    csv_files = [f for f in os.listdir(folder_path) if f.endswith('.csv')] 
    
    if not csv_files:
        print(f"No CSV files found in the directory: {folder_path}")
        return
    
    line_styles = ['-', '--', '-.', ':']  # Стили линий
    num_line_styles = len(line_styles)
    data_dict = {}
    
    for csv_file in csv_files:
        file_path = os.path.join(folder_path, csv_file)
        df = pd.read_csv(file_path)
        k_value = int(csv_file.split('x')[1].split('.')[0])
        data_dict[k_value] = df
    
    sorted_k_values = sorted(data_dict.keys())
    
    # Первый график: Error Rate vs Sigma
    plt.figure(figsize=(12, 8))  # Создаем первое окно
    for i, k_value in enumerate(sorted_k_values):
        df = data_dict[k_value]
        line_style = line_styles[i % num_line_styles]
        plt.plot(df['sigma'], df['error_rate'], linestyle=line_style, label=f'20x{k_value}')
    
    plt.title('Error Rate vs Sigma')
    plt.xlabel('Sigma')
    plt.ylabel('Error Rate')
    plt.legend(title='Codeword Size', bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.grid(True)
    
    # Сохранение и вывод первого графика
    output_file_1 = os.path.join(folder_path, 'error_rate_vs_sigma.png')
    plt.savefig(output_file_1, bbox_inches='tight')
    print(f"First plot saved to {output_file_1}")
    plt.show()
    
    # Второй график: Time vs k для всех сигм
    plt.figure(figsize=(12, 8))  # Создаем второе окно
    for i, sigma in enumerate(data_dict[sorted_k_values[0]]['sigma']):  # Берем сигмы из первого файла
        times_for_sigma = [data_dict[k_value].iloc[i]['time'] for k_value in sorted_k_values]
        line_style = line_styles[i % num_line_styles]  # Используем разные стили линий
        plt.plot(sorted_k_values, times_for_sigma, linestyle=line_style, label=f'Sigma={sigma:.1f}')
    
    plt.title('Time vs k for All Sigmas')
    plt.xlabel('k')
    plt.ylabel('Time')
    plt.legend(title='Sigma Values', bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.grid(True)
    
    # Сохранение и вывод второго графика
    output_file_2 = os.path.join(folder_path, 'time_vs_k.png')
    plt.savefig(output_file_2, bbox_inches='tight')
    print(f"Second plot saved to {output_file_2}")
    plt.show()

if __name__ == "__main__":
    import sys 
    
    if len(sys.argv) != 2:
        print("Usage: python visualize.py <path_to_folder>")
        sys.exit(1)
    
    folder_path = sys.argv[1]
    plot_simulation_results(folder_path)