#!/usr/bin/env python

'''
Builds a set of facet plots to have an overview of the FPGA FW performance.
Takes a folder where to save the plots and where the csv dataframe is stored.
'''

import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

folder = "reports_unroll"

df = pd.read_csv(folder + "/hls_main_csynth_dataframe.csv")

filtered_df = df[(df.PHI_JET_SIZE == df.ETA_JET_SIZE) & (df.ETA_GRID_SIZE >= df.ETA_JET_SIZE) & (df.PHI_GRID_SIZE >= df.PHI_JET_SIZE)]

#filtered_df = df[["PHI_JET_SIZE", "ETA_GRID_SIZE", "PHI_GRID_SIZE", "NUMBER_OF_SEEDS", "maxOverallLatency"]]

sns.set_style("whitegrid")

facet = sns.FacetGrid(filtered_df, row="NUMBER_OF_SEEDS", col="PHI_JET_SIZE", hue="PHI_GRID_SIZE")
facet = facet.map(sns.lineplot, "ETA_GRID_SIZE", "minOverallLatency", ci=None).add_legend()
facet.fig.set_size_inches(16, 9)
facet.savefig(folder + "/minOverallLatency.pdf")

facet = sns.FacetGrid(filtered_df, row="NUMBER_OF_SEEDS", col="PHI_JET_SIZE", hue="PHI_GRID_SIZE")
facet = facet.map(sns.lineplot, "ETA_GRID_SIZE", "maxOverallLatency", ci=None).add_legend()
facet.fig.set_size_inches(16, 9)
facet.savefig(folder + "/maxOverallLatency.pdf")

facet = sns.FacetGrid(filtered_df, row="NUMBER_OF_SEEDS", col="PHI_JET_SIZE", hue="ETA_GRID_SIZE")
facet = facet.map(sns.lineplot, "PHI_GRID_SIZE", "usedBRAM_18K", ci=None).add_legend()
facet.fig.set_size_inches(16, 9)
facet.savefig(folder + "/usedBRAM_18K.pdf")

facet = sns.FacetGrid(filtered_df, row="NUMBER_OF_SEEDS", col="PHI_JET_SIZE", hue="ETA_GRID_SIZE")
facet = facet.map(sns.lineplot, "PHI_GRID_SIZE", "usedDSP48E", ci=None).add_legend()
facet.fig.set_size_inches(16, 9)
facet.savefig(folder + "/usedDSP48E.pdf")

facet = sns.FacetGrid(filtered_df, row="NUMBER_OF_SEEDS", col="PHI_JET_SIZE", hue="ETA_GRID_SIZE")
facet = facet.map(sns.lineplot, "PHI_GRID_SIZE", "usedFF", ci=None).add_legend()
facet.fig.set_size_inches(16, 9)
facet.savefig(folder + "/usedFF.pdf")

facet = sns.FacetGrid(filtered_df, row="NUMBER_OF_SEEDS", col="PHI_JET_SIZE", hue="ETA_GRID_SIZE")
facet = facet.map(sns.lineplot, "PHI_GRID_SIZE", "usedLUT", ci=None).add_legend()
facet.fig.set_size_inches(16, 9)
facet.savefig(folder + "/usedLUT.pdf")

plt.show()