import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv('mutation_percent_exp.csv', delimiter='\s+')

n_cities = data['n_cities']
population_size = data['population_size']
mutation_percent = data['mutation_percent']
n_iterations = data['n_iterations']
time = data['time']
path_len = data['path_len']
accuracy = data['accuracy']

plt.figure(figsize=(10, 6))

plt.plot(mutation_percent, accuracy, label='T(MP)')

plt.xlabel('Процент мутаций')
plt.ylabel('Точность, %')
plt.title('Сходимость алгоритма при разном проценте мутаций\nКоличество точек: 100, Количество поколений: 10000, Особей в популяции: 4')
plt.legend()

plt.grid(True)
plt.savefig('mutation_percent_conv_graph.png')
