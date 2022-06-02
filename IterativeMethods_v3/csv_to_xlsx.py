import pandas as pd
import os.path
outData = pd.read_csv("table.csv", sep = ',', index_col=0, encoding='cp1251')
if(os.path.exists('table.xlsx')):
  os.remove('table.xlsx')
outData.drop(outData.columns[outData.iloc[0].size-1], axis = 1, inplace = True)
writer = pd.ExcelWriter('table.xlsx',engine='xlsxwriter') 
outData.to_excel(writer,'table') 
writer.save()
#outData