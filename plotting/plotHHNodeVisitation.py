import matplotlib.pyplot as plt
import numpy as np
import collections

HH14 = open("D:\Skrivebord\Bachelor Project\Code\TrackingFrequentDataItems\experimentresults\\heavyHittersNodesVisited.txt", "r")
HHDiff = open("D:\Skrivebord\Bachelor Project\Code\TrackingFrequentDataItems\experimentresults\\heavyHittersNodesVisitedDiff.txt", "r")

HH14Array = []
HHDiffArray = []

X = np.linspace(1, 100, 100)

for lines in HH14:
    HH14Array.append(int(lines))
for lines in HHDiff:
    HHDiffArray.append(int(lines))

HH14.close()
HHDiff.close()

newHH14array = sorted(HH14Array)
newHHDiffarray = sorted(HHDiffArray)

fig, ax = plt.subplots(figsize=(12,8))
plt.plot(X, newHH14array, label="HHModern")
plt.plot(X, newHHDiffarray, label="HHClassic")
plt.ylabel("Running time in MS", fontdict={'family':'serif','color':'black','size':20})
plt.xlabel("Amount of counters k, k=k^x", fontdict={'family':'serif','color':'black','size':20})
plt.xlim([0, 100])
plt.yticks(np.arange(145, 181, 1))
plt.legend()
plt.show()