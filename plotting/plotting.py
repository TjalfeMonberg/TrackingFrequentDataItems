import matplotlib.pyplot as plt
import collections

file = open("../plotfiles/plotFileCS.txt", "r")

resArray = []

for lines in file:
    resArray.append(int(lines))
    
file.close()

counter = collections.Counter(resArray)
newArray = sorted(counter.items())

def splitListOfTuples(lst):
    lst1 = []
    lst2 = []
    for x, y in lst:
        lst1.append(x)
        lst2.append(y)
    return (lst1, lst2)

xPoints, yPoints = splitListOfTuples(newArray)

X_ = xPoints
Y_ = yPoints

fig, ax = plt.subplots(figsize=(12,8))

plt.bar(X_, Y_)
for index in range(len(X_)):
  plt.text(X_[index], Y_[index], Y_[index], size = 7, ha = 'center')
plt.show()