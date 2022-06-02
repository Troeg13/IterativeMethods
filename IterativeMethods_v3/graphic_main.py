import pandas as pd
import os.path
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
import seaborn as sns

Data = pd.read_csv("table.csv", sep = ',', header=None, index_col=None, encoding='cp1251')
Data.drop(Data.columns[Data.iloc[0].size-1], axis = 1, inplace = True)
x = np.zeros(Data.iloc[0].size-1)
for i in range(0, Data.iloc[0].size-1):
    x[i] = Data[i+1][0]
y = np.zeros(Data.shape[0] - 1)
for j in range(0, Data.shape[0] - 1):
    y[j] = Data[0][j+1]
Data.drop(Data.columns[Data.index[0]], inplace = True) #значения узлов по х
Data.drop(columns = 0, inplace = True)
Data = Data.to_numpy()
x = x.astype('float64')
y = y.astype('float64')
Data = Data.astype('float64')

fig = plt.figure(figsize = (6, 6)) #создаём трёхмерную систему координат
ax = fig.add_subplot(projection = '3d')
X, Y = np.meshgrid(x, y)
ax.plot_surface(X, Y, Data, cmap=sns.cubehelix_palette(200, 5, as_cmap=True, reverse=True))
ax.set_facecolor('xkcd:salmon')
ax.set_facecolor((0.66, 0.64, 0.64))
fig.set_facecolor('xkcd:salmon')
fig.set_facecolor((0.66, 0.64, 0.64))
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('V(x,y)')
ax.set_title('График численного решения V(x,y), полушенного на шаге N')

#точное решение
Data_u = pd.read_csv("table_v2.csv", sep = ',', header=None, index_col=None, encoding='cp1251')
Data_u.drop(Data_u.columns[Data_u.iloc[0].size-1], axis = 1, inplace = True)
x_u = np.zeros(Data_u.iloc[0].size-1)
for i in range(0, Data_u.iloc[0].size-1):
    x_u[i] = Data_u[i+1][0]
y_u = np.zeros(Data_u.shape[0] - 1)
for j in range(0, Data_u.shape[0] - 1):
    y_u[j] = Data_u[0][j+1]
Data_u.drop(Data_u.columns[Data_u.index[0]], inplace = True) #значения узлов по х
Data_u.drop(columns = 0, inplace = True)
Data_u = Data_u.to_numpy()
x_u = x_u.astype('float64')
y_u = y_u.astype('float64')
Data_u = Data_u.astype('float64')

fig_u = plt.figure(figsize = (6, 6)) #создаём трёхмерную систему координат
ax_2 = fig_u.add_subplot(projection = '3d')
X_u, Y_2 = np.meshgrid(x_u, y_u)
ax_2.plot_surface(X_u, Y_2, Data_u, cmap=sns.cubehelix_palette(200, 5, as_cmap=True, reverse=True))
ax_2.set_facecolor('xkcd:salmon')
ax_2.set_facecolor((0.66, 0.64, 0.64))
fig_u.set_facecolor('xkcd:salmon')
fig_u.set_facecolor((0.66, 0.64, 0.64))
ax_2.set_xlabel('x')
ax_2.set_ylabel('y')
ax_2.set_zlabel('V2(x,y)')
ax_2.set_title('График численного решения V2(x,y), полученного на шаге 2N')

#начальное приближение
Data_r0 = pd.read_csv("table_v0.csv", sep = ',', header=None, index_col=None, encoding='cp1251')
Data_r0.drop(Data_r0.columns[Data_r0.iloc[0].size-1], axis = 1, inplace = True)
x_r0 = np.zeros(Data_r0.iloc[0].size-1)
for i in range(0, Data_r0.iloc[0].size-1):
    x_r0[i] = Data_r0[i+1][0]
y_r0 = np.zeros(Data_r0.shape[0] - 1)
for j in range(0, Data_r0.shape[0] - 1):
    y_r0[j] = Data_r0[0][j+1]
Data_r0.drop(Data_r0.columns[Data_r0.index[0]], inplace = True) #значения узлов по х
Data_r0.drop(columns = 0, inplace = True)
Data_r0 = Data_r0.to_numpy()
x_r0 = x_r0.astype('float64')
y_r0 = y_r0.astype('float64')
Data_r0 = Data_r0.astype('float64')

fig_r0 = plt.figure(figsize = (6, 6)) #создаём трёхмерную систему координат
ax_v0 = fig_r0.add_subplot(projection = '3d')
X_v0, Y_v0 = np.meshgrid(x_r0, y_r0)
ax_v0.plot_surface(X_v0, Y_v0, Data_r0, cmap=sns.cubehelix_palette(200, 5, as_cmap=True, reverse=True))
ax_v0.set_facecolor('xkcd:salmon')
ax_v0.set_facecolor((0.66, 0.64, 0.64))
fig_r0.set_facecolor('xkcd:salmon')
fig_r0.set_facecolor((0.66, 0.64, 0.64))
ax_v0.set_xlabel('x')
ax_v0.set_ylabel('y')
ax_v0.set_zlabel('V(x,y)_0')
ax_v0.set_title('График начального приближения V(x, y)_0 ')

#разность точного и численного
Data_z = pd.read_csv("table_v20.csv", sep = ',', header=None, index_col=None, encoding='cp1251')
Data_z.drop(Data_z.columns[Data_z.iloc[0].size-1], axis = 1, inplace = True)
x_z = np.zeros(Data_z.iloc[0].size-1)
for i in range(0, Data_z.iloc[0].size-1):
    x_z[i] = Data_z[i+1][0]
y_z = np.zeros(Data_z.shape[0] - 1)
for j in range(0, Data_z.shape[0] - 1):
    y_z[j] = Data_z[0][j+1]
Data_z.drop(Data_z.columns[Data_z.index[0]], inplace = True) #значения узлов по х
Data_z.drop(columns = 0, inplace = True)
Data_z = Data_z.to_numpy()
x_z = x_z.astype('float64')
y_z = y_z.astype('float64')
Data_z = Data_z.astype('float64')

#Data = np.transpose(Data)
fig_z = plt.figure(figsize = (6, 6)) #создаём трёхмерную систему координат
ax_z = fig_z.add_subplot(projection = '3d')
X_z, Y_z = np.meshgrid(x_z, y_z)
ax_z.plot_surface(X_z, Y_z, Data_z, cmap=sns.cubehelix_palette(200, 5, as_cmap=True, reverse=True))
ax_z.set_facecolor('xkcd:salmon')
ax_z.set_facecolor((0.66, 0.64, 0.64))
fig_z.set_facecolor('xkcd:salmon')
fig_z.set_facecolor((0.66, 0.64, 0.64))
#ax.plot_surface(X, Y, Data, cmap=cm.coolwarm)
ax_z.set_xlabel('x')
ax_z.set_ylabel('y')
ax_z.set_zlabel('V2(x,y)_0')
ax_z.set_title('График начального приближения V2(x, y)_0')


Data_r = pd.read_csv("table_ra.csv", sep = ',', header=None, index_col=None, encoding='cp1251')
Data_r.drop(Data_r.columns[Data_r.iloc[0].size-1], axis = 1, inplace = True)
x_r = np.zeros(Data_r.iloc[0].size-1)
for i in range(0, Data_r.iloc[0].size-1):
    x_r[i] = Data_r[i+1][0]
y_r = np.zeros(Data_r.shape[0] - 1)
for j in range(0, Data_r.shape[0] - 1):
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
ax_r.plot_surface(X_2, Y_2, Data_r, cmap=sns.cubehelix_palette(200, 5, as_cmap=True, reverse=True))
ax_r.set_facecolor('xkcd:salmon')
ax_r.set_facecolor((0.66, 0.64, 0.64))
fig_r.set_facecolor('xkcd:salmon')
fig_r.set_facecolor((0.66, 0.64, 0.64))
ax_r.set_xlabel('x')
ax_r.set_ylabel('y')
ax_r.set_zlabel('V(x,y) - V2(x,y)', labelpad = 15)
ax_r.set_title('График V(x,y) - V2(x,y) где V - численное реш-е, V2 - численное реш-е с половинным шагом')
plt.show()