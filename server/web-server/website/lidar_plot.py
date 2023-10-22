import math
from matplotlib import pyplot as plt

def plot():
    plt.rcParams["figure.figsize"] = [25.00, 12.50]
    plt.rcParams["figure.autolayout"] = True

    plt.xlim(-10000, 10000)
    plt.ylim(-10000, 10000)

    plt.grid()

    lidar_data = '' 
    with open('lidar_data.txt', 'r') as file:
        lidar_data = file.read()

    val = ''
    x = 0
    y = 0

    for c in lidar_data:
        if c == '-':
            val += c
        elif c.isnumeric():
            val += c
        elif c == ',':
            x = int(val)
            val = ''
        if c == '\n':
            y = int(val)
            plt.plot(x, y, marker=".", markersize=5, color="red")
            val = ''

    plt.show()

if __name__ == "__main__":
    plot()