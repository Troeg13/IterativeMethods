import pandas as pd
import os.path
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
import seaborn as sns

Data_r = pd.read_csv("table_ra.csv", sep = ',', header=None, index_col=None, encoding='cp1251')
Data_r.drop(Data_r.columns[Data_r.iloc[0].size-1], axis = 1, inplace = True)
x_r = np.zeros(Data_r.iloc[0].size-1)
for i in range(0, Data_r.iloc[0].size-1):
    x_r[i] = Data_r[i+1][0]
y_r = np.zeros(Data_r.iloc[1].size-1)
for j in range(0, Data_r.iloc[1].size-1):
    y_r[j] = Data_r[0][j+1]
Data_r.drop(Data_r.columns[Data_r.index[0]], inplace = True) #значения узлов по х
Data_r.drop(columns = 0, inplace = True)
Data_r = Data_r.to_numpy()
x_r = x_r.astype('float64')
y_r = y_r.astype('float64')
Data_r = Data_r.astype('float64')

fig_r = plt.figure(figsize = (10, 7)) #создаём трёхмерную систему координат
ax_r = fig_r.add_subplot(projection = '3d')
X_2, Y_2 = np.meshgrid(x_r, y_r)
ax_r.plot_surface(X_2, Y_2, Data_r, cmap=sns.diverging_palette(200, 2, s = 80, center ='dark', as_cmap=True))
ax_r.set_facecolor('xkcd:salmon')
ax_r.set_facecolor((0.66, 0.64, 0.64))
fig_r.set_facecolor('xkcd:salmon')
fig_r.set_facecolor((0.66, 0.64, 0.64))
ax_r.set_xlabel('x')
ax_r.set_ylabel('y')
ax_r.set_zlabel('V(x,y) - V2(x,y)')
ax_r.set_title('График V(x,y) - V2(x,y) где V - численное реш-е, V2 - численное реш-е с половинным шагом')
plt.show()