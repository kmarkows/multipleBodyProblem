import numpy as np
import matplotlib.pyplot as plt

np.position = []
with open('./data/positionData.txt', 'r') as file:
    for line in file:
        np.position.append([float(number) for number in line.split()])

t = 0
for pos in np.position:
    np.bodiesX = []
    np.bodiesY = []
    for i in range((len(pos))):
        if (i%2 == 0):
            np.bodiesX.append(pos[i])
        else:
            np.bodiesY.append(pos[i])

    if ( t < 10):
        plotSavingDirectory = "./data/plotsForGif/positions000"
        plotSavingDirectory += str(t)

    elif ( t >= 10 and t < 100):
        plotSavingDirectory = "./data/plotsForGif/positions00"
        plotSavingDirectory += str(t)

    elif ( t >= 100 and t < 1000):
        plotSavingDirectory = "./data/plotsForGif/positions0"
        plotSavingDirectory += str(t)

    plt.scatter(np.bodiesX, np.bodiesY)
    plt.xlim(-2, 2)
    plt.ylim(-2, 2)
    plt.savefig(plotSavingDirectory)
    plt.close()
    t += 1
