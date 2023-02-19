
# importing the required module
import pandas as pds
import matplotlib.pyplot as plt


fileName = ""


dataFrame = pds.read_csv(fileName)
dataFrame.plot(x='Time (s)', y='Temperature (F)', kind='line')
plt.ylabel('Temperature (F)')
plt.show()
