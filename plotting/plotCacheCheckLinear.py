import matplotlib.pyplot as plt
import numpy as np
import collections

CMS = open("D:\Skrivebord\Bachelor Project\Code\TrackingFrequentDataItems\experimentresults\\runAndQueryTimeCMS1to100.txt", "r")
CS = open("D:\Skrivebord\Bachelor Project\Code\TrackingFrequentDataItems\experimentresults\\runAndQueryTimeCS1to100.txt", "r")
MG = open("D:\Skrivebord\Bachelor Project\Code\TrackingFrequentDataItems\experimentresults\\runAndQueryTimeMG1to100.txt", "r")

CMSArray = []
CSArray = []
MGArray = []

X = np.linspace(0, 100, 100)

for lines in CMS:
    CMSArray.append(int(lines))
for lines in CS:
    CSArray.append(int(lines))
for lines in MG:
    MGArray.append(int(lines))

CMS.close()
CS.close()
MG.close()
newCMSarray = sorted(CMSArray)
newCSarray = sorted(CSArray)
newMGarray = sorted(MGArray)

fig, ax = plt.subplots(figsize=(12,8))
plt.plot(X, newCMSarray, label="CMS")
plt.plot(X, newCSarray, label="CS")
plt.plot(X, newMGarray, label="MG")
plt.ylabel("Running time in MS", fontdict={'family':'serif','color':'black','size':20})
plt.xlabel("Amount of counters k, k=k^x", fontdict={'family':'serif','color':'black','size':20})
plt.legend()
plt.show()