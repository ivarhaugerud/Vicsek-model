import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation
import pandas as pd
import seaborn as sns
import os

plt.style.use("bmh")
sns.color_palette("hls", 1)

import matplotlib
matplotlib.rc('xtick', labelsize=14)
matplotlib.rc('ytick', labelsize=14)
matplotlib.rcParams['mathtext.fontset'] = 'stix'
matplotlib.rcParams['font.family'] = 'STIXGeneral'


def update_quiver(num):
    global Q
    Q.remove()

    state = get_data("data/test" +str(num) + ".txt", ["x", "y", "angle", "vx", "vy"])
    Q = ax.quiver(state["x"], state["y"], state["vx"], state["vy"], pivot='mid', units='inches')

    return Q

def get_data(filename, variables):
    df = pd.read_csv(filename,\
                    delim_whitespace=True, \
                    engine='python', \
                    names=variables)
    return df
    #using pandas to read the data files



number_of_files = 1000

fig, ax = plt.subplots(1,1)
state = get_data("data/test0.txt", ["x", "y", "angle", "vx", "vy"])
Q = ax.quiver(state["x"], state["y"], state["vx"], state["vy"], pivot='mid', units='inches')


anim = animation.FuncAnimation(fig, update_quiver, frames=np.linspace(0, number_of_files-1, number_of_files, dtype="int"),
                               interval=100, blit=False, repeat=False)
plt.show()
