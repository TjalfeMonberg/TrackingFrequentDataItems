import numpy as np
import matplotlib.pyplot as plt
from scipy.special import zeta
import os

def createDataSet():
    file = open('customDataSet.txt', 'w')
    dataSet = np.random.zipf(a=2, size=1000)

    for i in range(len(dataSet)):
        file.write("%d \n"  %dataSet[i])

    file.close()

def mostFrequnet(List):
    unique, counts = np.unique(List, return_counts=True)
    index = np.argmax(counts)
    return unique[index]

def countOfMostFrequent(List):
    unique = mostFrequnet(List)
    counter = 0
    for x in List:
        if x == unique:
            counter = counter + 1
    return counter

#createDataSet()
'''
x = np.loadtxt('./plotfiles/customDataSet.txt')
mostCounts = countOfMostFrequent(x)
largest = max(x)
print('Largest: ', largest)
print('Most frequent: ', mostCounts)
sns.displot(x[x <= largest], kde=True)

plt.show() 
'''

s= np.loadtxt(r'C:\Users\Ruben\Desktop\Uni\6. semester\Bachelorprojekt\trainingDatasets\dataset12.txt').astype(np.int64)
a = 1.2
n = 10000000


bincount = np.bincount(s)
k = np.arange(1, s.max() + 1)
plt.bar(k, bincount[1:], label='sample count')
plt.plot(k, n*(k**-a)/zeta(a), 'k.-', label='expected count')   
plt.semilogy()
plt.grid(alpha=0.4)
plt.legend()
plt.title(f'Zipf sample, a={a}, size={n}')
plt.show()