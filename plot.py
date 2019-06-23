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

data_100 = get_data("data/new_run_100.txt", ["L", "R", "rho", "v0", "eta", "order"])
data_50 = get_data("data/new_run_50.txt", ["L", "R", "rho", "v0", "eta", "order"])
data_30 = get_data("data/new_run_30.txt", ["L", "R", "rho", "v0", "eta", "order"])
data_10 = get_data("data/new_run_10.txt", ["L", "R", "rho", "v0", "eta", "order"])
data_1 = get_data("data/new_run_1.txt", ["L", "R", "rho", "v0", "eta", "order"])

plt.plot(data_100["eta"], data_100["order"], "o", label=r"$\rho = 100$")
plt.plot(data_50["eta"], data_50["order"], "o", label=r"$\rho = 50$")
plt.plot(data_30["eta"], data_30["order"], "o", label=r"$\rho = 30$")
plt.plot(data_10["eta"], data_10["order"], "o", label=r"$\rho = 10$")
plt.plot(data_1["eta"], data_1["order"], "o", label=r"$\rho = 1$")

plt.legend(loc="best")
plt.show()
