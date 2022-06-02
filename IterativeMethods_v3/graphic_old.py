import pandas as pd
import os.path
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
Data = pd.read_csv("table.csv", sep = ',', index_col=0, encoding='cp1251')
Data.drop(Data.index[[0]], inplace = True)
Data.drop(Data.columns[Data.iloc[0].size-1], axis = 1, inplace = True)
Data = Data.to_numpy()
Data = Data.astype('float64')
T = np.loadtxt("T.txt", dtype = np.float64)
m = Data[1].size - 1
size = Data.size - 1
Data = np.transpose(Data)
fig = plt.figure(figsize = (7, 4))
ax = fig.add_subplot(projection = '3d')
X = Data[0]
Y = np.linspace(0, T, m)
x, y = np.meshgrid(X, Y)
z = Data[1:Data[1:].size]
ax.plot_surface(x, y, z, cmap=cm.coolwarm)
ax.set_xlabel('x (сечение)')
ax.set_ylabel('t (время)')
ax.set_zlabel('V (температура)')
ax.set_title('График температур')
plt.show()