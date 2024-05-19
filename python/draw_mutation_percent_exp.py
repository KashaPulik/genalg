import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv('/home/pahansan/genalg/data/mutation_percent_exp.csv', delimiter='\s+')

n_cities = data['n_cities']
population_size = data['population_size']
mutation_percent = data['mutation_percent']
n_iterations = data['n_iterations']
time = data['time']
path_len = data['path_len']
accuracy = data['accuracy']

plt.figure(figsize=(10, 6))

plt.plot(mutation_percent, time, label='T(MP)')

plt.xlabel('Процент мутаций')
plt.ylabel('Время, с')
plt.title('Зависимость времени выполнения от процента мутаций\nКоличество точек: 100, Количество поколений: 10000, Особей в популяции: 4')
plt.legend()

plt.grid(True)
plt.savefig('/home/pahansan/genalg/time_graphs/mutation_percent_graph.png')
