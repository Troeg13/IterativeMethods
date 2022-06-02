import pandas as pd
import os.path
import matplotlib.pyplot as plt
import numpy as np
data = pd.read_csv("table.csv", sep = ',', index_col=0, encoding='cp1251')
data = data.to_numpy()
data = np.transpose(data)
number = np.loadtxt("number.txt", dtype = np.int32)
if number.size == 1:
  fig, ax = plt.subplots()
  ax.plot(data[0], data[number])
  ax.set_xlabel('x (сечение)')
  ax.set_ylabel('V (температура)')
  number -= 1
  number_str = 'Слой № ' + str(number)
  ax.set_title(number_str)
else:
  for i in range(0, number.size, 1):
    fig, ax = plt.subplots()
    ax.plot(data[0], data[number[i]])
    ax.set_xlabel('x (сечение)')
    ax.set_ylabel('V (температура)')
    number_str = 'Слой № ' + str(number[i] - 1)
    ax.set_title(number_str)
plt.show()