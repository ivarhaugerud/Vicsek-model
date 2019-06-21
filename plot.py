import numpy as np
import matplotlib.pyplot as plt
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

def get_data(filename, variables):
    df = pd.read_csv(filename,\
                    delim_whitespace=True, \
                    engine='python', \
                    names=variables)
    return df
    #using pandas to read the data files


amount_of_data = 1000
velocity_sum_x = np.zeros(amount_of_data)
velocity_sum_y = np.zeros(amount_of_data)
angle_sum      = np.zeros(amount_of_data)

for i in range(amount_of_data):
    state = get_data("data/test"+str(i)+".txt", ["x", "y", "angle", "vx", "vy"])

    velocity_sum_x[i] = np.sum(state["vx"]/50)
    velocity_sum_y[i] = np.sum(state["vy"]/50)
    angle_sum[i]      = np.sum(state["angle"])/(2*np.pi*100)

plt.plot(velocity_sum_x**2 + velocity_sum_y**2)
plt.xlabel("Timesteps")
plt.ylabel("Order parameter")
plt.show()

plt.plot(angle_sum)
plt.xlabel("Timesteps")
plt.ylabel("Order parameter")
plt.show()
