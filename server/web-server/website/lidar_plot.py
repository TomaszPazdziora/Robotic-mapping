import math
from matplotlib import pyplot as plt
import numpy as np

def plot_lidar_data():
    plt.rcParams["figure.figsize"] = [25.00, 12.50]
    plt.rcParams["figure.autolayout"] = True

    plt.xlim(-5000, 5000)
    plt.ylim(-5000, 5000)

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
            plt.plot(x, y, marker=".", markersize=4, color="red")
            # plt.plot(x, y, marker="s", markersize=10, color="blue")
            val = ''

    plt.show()

def plot_occupancy():
    plt.rcParams["figure.figsize"] = [25.00, 12.50]
    plt.rcParams["figure.autolayout"] = True

    plt.xlim(0, 250)
    plt.ylim(0, 250)

    plt.grid()

    lidar_data = '' 
    with open('occupancy.txt', 'r') as file:
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
            plt.plot(x, y, marker="s", markersize=10, color="blue")
            val = ''

    plt.show()


if __name__ == "__main__":
    plot_lidar_data()
    # plot_occupancy()