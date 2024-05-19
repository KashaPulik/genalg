import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv('/home/pahansan/genalg/data/n_iterations_exp.csv', delimiter='\s+')

n_cities = data['n_cities']
population_size = data['population_size']
mutation_percent = data['mutation_percent']
n_iterations = data['n_iterations']
time = data['time']
path_len = data['path_len']
accuracy = data['accuracy']

plt.figure(figsize=(10, 6))

plt.plot(n_iterations, time, label='T(N)')

plt.xlabel('Количество поколений N')
plt.ylabel('Время, с')
plt.title('Зависимость времени выполнения от количества поколений\nПроцент мутаций: 100%, Количество городов: 100, Особей в популяции: 4')
plt.legend()

plt.grid(True)
plt.savefig('/home/pahansan/genalg/time_graphs/n_iterations_graph.png')
