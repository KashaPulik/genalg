import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv('population_size_exp.csv', delimiter='\s+')

n_cities = data['n_cities']
population_size = data['population_size']
mutation_percent = data['mutation_percent']
n_iterations = data['n_iterations']
time = data['time']
path_len = data['path_len']
accuracy = data['accuracy']

plt.figure(figsize=(10, 6))

plt.plot(population_size, accuracy, label='T(N)')

plt.xlabel('Количество особей N')
plt.ylabel('Точность, %')
plt.title('Сходимость алгоритма при разных размерах популяции\nПроцент мутаций: 100%, Количество городов: 100, Количество поколений: 10000')
plt.legend()

plt.grid(True)
plt.savefig('population_size_conv_graph.png')
