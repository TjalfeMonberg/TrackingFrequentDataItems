import numpy as np
import matplotlib.pyplot as plt
from scipy.special import zeta
import os
import random
from collections import Counter

def randomGen():
    f = open('CustomDataset.txt', 'w')
    List = []
    for _ in range(10000000):
        List.append(random.randint(1, 10000))
    for item in List:
        f.write("%s\n" %item)
    f.close()

def npRandomGen():
    f = open("CustomDataset.txt", 'w')
    List = list(np.random.randint(low = 0, high=10000, size=10000000))
    for item in List:
        f.write("%s\n" %item)
    f.close()

def genVisual():
    path = os.path.realpath(__file__)
    dir = os.path.dirname(path)
    dir = dir.replace('plotting', 'trainingDatasets')
    os.chdir(dir)
    s= np.loadtxt('CustomDataset.txt').astype(np.int64)
    a = 1.0
    n = 10000000


    bincount = np.bincount(s)
    k = np.arange(1, s.max() + 1)
    plt.bar(k, bincount[1:], label='sample count')
    #plt.plot(k, n*(k**-a)/zeta(a), 'k.-', label='expected count')   
    plt.plot(k, n*(k**-a)/np.sum(k**-a), 'k.-', label='expected count')   
    plt.semilogy()
    plt.grid(alpha=0.4)
    plt.legend()
    plt.title(f'Zipf sample, a={a}, size={n}')
    plt.show()

