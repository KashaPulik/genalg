import pandas as pd
import matplotlib.pyplot as plt

# Загрузка данных из файла с указанием разделителя столбцов
data = pd.read_csv('data.csv', delimiter='\s+')

# Извлечение данных из столбцов
n_cities = data['n_cities']
population_size = data['population_size']
mutation_percent = data['mutation_percent']
n_iterations = data['n_iterations']
time = data['time']
path_len = data['path_len']
accuracy = data['accuracy']

# Создание графика
plt.figure(figsize=(10, 6))

# Построение графиков
# plt.plot(n_iterations, time, label='Time')
# plt.plot(n_iterations, path_len, label='Path Length')
# plt.plot(n_iterations, accuracy, label='Accuracy')

plt.plot(mutation_percent, time, label='mutation_percent')

# Добавление легенды и меток осей
plt.xlabel('mutation_percent')
plt.ylabel('time')
plt.title('ABOBA')
plt.legend()

# Отображение графика
plt.grid(True)
plt.savefig('mutation_percent_graph.png')
