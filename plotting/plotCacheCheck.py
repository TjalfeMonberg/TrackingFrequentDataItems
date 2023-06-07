import matplotlib.pyplot as plt
import numpy as np
import collections

CMS = open("D:\Skrivebord\Bachelor Project\Code\TrackingFrequentDataItems\experimentresults\\runAndQueryTimeCMS1to40.txt", "r")
CS = open("D:\Skrivebord\Bachelor Project\Code\TrackingFrequentDataItems\experimentresults\\runAndQueryTimeCS1to40.txt", "r")
MG = open("D:\Skrivebord\Bachelor Project\Code\TrackingFrequentDataItems\experimentresults\\runAndQueryTimeMG1to40.txt", "r")

CMSArray = []
CSArray = []
MGArray = []

max = 2**28
middle = 2**14
X = np.linspace(1, 22, 22)

for lines in CMS:
    CMSArray.append(int(lines))
for lines in CS:
    CSArray.append(int(lines))
for lines in MG:
    MGArray.append(int(lines))

CMS.close()
CS.close()
MG.close()

newCMSarray = CMSArray[0:22]
newCSarray = CSArray[0:22]
newMGarray = MGArray[0:22]
fig, ax = plt.subplots(figsize=(12,8))
plt.plot(X, newCMSarray, label="CMS")
plt.plot(X, newCSarray, label="CS")
plt.plot(X, newMGarray, label="MG")
plt.ylabel("Running time in MS", fontdict={'family':'serif','color':'black','size':20})
plt.xlabel("Amount of counters k, k=k^x", fontdict={'family':'serif','color':'black','size':20})
plt.xlim([1, 22])
plt.xticks(np.arange(1, 23, 1))
plt.legend()
plt.show()