import matplotlib.pyplot as plt

matrix_sizes = [500, 1000, 1500, 2000, 2500]

time_1_pthread = [707.31, 7698.68, 24412.58, 58705.72, 167031.81]
time_4_pthread = [174.70, 1789.42, 5733.46, 13919.28, 43258.75]
time_16_pthread = [104.06, 1056.68, 3201.04, 7315.89, 28327.13]
time_64_pthread = [97.12, 1019.07, 3159.84, 7254.32, 27313.58]
time_128_pthread = [99.23, 1015.23, 3127.55, 7194.61, 27295.28]

plt.figure(figsize=(10, 6))

plt.plot(matrix_sizes, time_1_pthread, marker='o', linewidth=2, label="1 поток")
plt.plot(matrix_sizes, time_4_pthread, marker='s', linewidth=2, label="4 потока")
plt.plot(matrix_sizes, time_16_pthread, marker='^', linewidth=2, label="16 потоков")
plt.plot(matrix_sizes, time_64_pthread, marker='d', linewidth=2, label="64 потока")
plt.plot(matrix_sizes, time_128_pthread, marker='x', linewidth=2, label="128 потоков")

plt.title("Зависимость времени умножения матриц от их размера")
plt.xlabel("Размер матрицы (N x N)")
plt.ylabel("Время выполнения (мс)")
plt.grid(True, linestyle='--', alpha=0.7)
plt.legend()

plt.savefig("matrix_perf_graph.png", dpi=300)
plt.show()
