import pandas as pd
import numpy as np
import os
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


FILE = "csv.txt"

def main():
    df = pd.read_csv(os.path.join(os.getcwd(), FILE), sep= ";")
    agg_df = df[["K", "time", 'worktime']].groupby(['K', 'worktime']).sum()
    fig = plt.figure(figsize=(8, 6))
    ax = fig.add_subplot(111, projection='3d')
    ax.plot_trisurf(agg_df.index.get_level_values('K'), 
                agg_df.index.get_level_values('worktime'), 
                agg_df['time'], cmap='viridis')


    ax.set_xlabel('K')
    ax.set_ylabel('Worktime')
    ax.set_zlabel('Time')

    ax.set_xticks(range(agg_df.index.get_level_values('K').min(), agg_df.index.get_level_values('K').max()+1, 1))
    ax.set_yticks(range(agg_df.index.get_level_values('worktime').min(), agg_df.index.get_level_values('worktime').max()+1, 1))

    temp = agg_df.idxmin()[0]
    print(f"best k is {temp}")
    plt.show()


main()