import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import make_interp_spline, BSpline
import collections

CMS = open("D:\Skrivebord\Bachelor Project\Code\TrackingFrequentDataItems\experimentresults\\averageErrorCMS.txt", "r")
CS = open("D:\Skrivebord\Bachelor Project\Code\TrackingFrequentDataItems\experimentresults\\averageErrorCSWithProperT.txt", "r")
MG = open("D:\Skrivebord\Bachelor Project\Code\TrackingFrequentDataItems\experimentresults\\averageErrorMG.txt", "r")

CMSArray = []
CSArray = []
MGArray = []


for lines in CMS:
    CMSArray.append(int(lines))
for lines in CS:
    CSArray.append(int(lines))
for lines in MG:
    MGArray.append(int(lines))

CMS.close()
CS.close()
MG.close()
counterCMS = collections.Counter(CMSArray)
counterCS = collections.Counter(CSArray)
counterMG = collections.Counter(MGArray)
newArrayCMS = sorted(CMSArray)
newArrayCS = sorted(CSArray)
newArrayMG = sorted(MGArray)

def splitListOfTuples(lst):
    lst1 = []
    lst2 = []
    for x, y in lst:
        lst1.append(x)
        lst2.append(y)
    return (lst1, lst2)
blub = np.arange(101)
x = np.linspace(0, 100, 50000)
newArray = np.array(newArrayCMS)
spl = make_interp_spline(x, newArray, k=3)
test = spl(x)
fig, ax = plt.subplots(figsize=(12,8))
print(x.shape)
print(newArray.shape)
plt.plot(x, test, label="CMS")
plt.plot(x, newArrayCS, label="CS")
plt.ylabel("Value of Error", fontdict={'family':'serif','color':'black','size':20})
plt.xlabel("Percentile; Lowest to highest error", fontdict={'family':'serif','color':'black','size':20})
plt.xticks(np.arange(0, 101, 10))
ax.text(50, newArrayCMS[24999]+500, "%d" %newArrayCMS[24999], ha="center")
ax.text(0, newArrayCMS[0]+1000, "%d" %newArrayCMS[0], ha="center")
ax.text(100, newArrayCMS[49999]+500, "%d" %newArrayCMS[49999], ha="center")
ax.text(50, newArrayCS[24999]+500, "%d" %newArrayCS[24999], ha="center")
ax.text(0, newArrayCS[0]+1000, "%d" %newArrayCS[0], ha="center")
ax.text(100, newArrayCS[49999]+500, "%d" %newArrayCS[49999], ha="center")
plt.legend()
plt.show()