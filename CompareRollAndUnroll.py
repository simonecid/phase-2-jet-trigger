#!/usr/bin/env python

import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

rolled_dataframe_path = "reports/hls_main_csynth_dataframe.csv"
unrolled_dataframe_path = "reports_unroll/hls_main_csynth_dataframe.csv"

saveFolder = "."

df_unroll = pd.read_csv(unrolled_dataframe_path)
df_roll = pd.read_csv(rolled_dataframe_path)

df_unroll["LOOP_TYPE"] = "UNROLLED"
df_roll["LOOP_TYPE"] = "ROLLED"
df = df_roll.append(df_unroll)

filtered_df = df[(df.PHI_JET_SIZE == df.ETA_JET_SIZE) & (df.ETA_GRID_SIZE >= df.ETA_JET_SIZE) & (df.PHI_GRID_SIZE >= df.PHI_JET_SIZE) & (df.NUMBER_OF_SEEDS==128)]

#filtered_df = df[["PHI_JET_SIZE", "ETA_GRID_SIZE", "PHI_GRID_SIZE", "NUMBER_OF_SEEDS", "maxOverallLatency"]]

sns.set_style("whitegrid")

facet = sns.FacetGrid(filtered_df, row="ETA_GRID_SIZE", col="PHI_JET_SIZE", hue="LOOP_TYPE")
facet = facet.map(sns.lineplot, "PHI_GRID_SIZE", "minOverallLatency", ci=None).add_legend()
facet.fig.set_size_inches(16, 9)
facet.savefig(saveFolder + "/minOverallLatency.pdf")

facet = sns.FacetGrid(filtered_df, row="ETA_GRID_SIZE", col="PHI_JET_SIZE", hue="LOOP_TYPE")
facet = facet.map(sns.lineplot, "PHI_GRID_SIZE", "maxOverallLatency", ci=None).add_legend()
facet.fig.set_size_inches(16, 9)
facet.savefig(saveFolder + "/maxOverallLatency.pdf")

facet = sns.FacetGrid(filtered_df, row="ETA_GRID_SIZE", col="PHI_JET_SIZE", hue="LOOP_TYPE")
facet = facet.map(sns.lineplot, "PHI_GRID_SIZE", "usedBRAM_18K", ci=None).add_legend()
facet.fig.set_size_inches(16, 9)
facet.savefig(saveFolder + "/usedBRAM_18K.pdf")

facet = sns.FacetGrid(filtered_df, row="ETA_GRID_SIZE", col="PHI_JET_SIZE", hue="LOOP_TYPE")
facet = facet.map(sns.lineplot, "PHI_GRID_SIZE", "usedDSP48E", ci=None).add_legend()
facet.fig.set_size_inches(16, 9)
facet.savefig(saveFolder + "/usedDSP48E.pdf")

facet = sns.FacetGrid(filtered_df, row="ETA_GRID_SIZE", col="PHI_JET_SIZE", hue="LOOP_TYPE")
facet = facet.map(sns.lineplot, "PHI_GRID_SIZE", "usedFF", ci=None).add_legend()
facet.fig.set_size_inches(16, 9)
facet.savefig(saveFolder + "/usedFF.pdf")

facet = sns.FacetGrid(filtered_df, row="ETA_GRID_SIZE", col="PHI_JET_SIZE", hue="LOOP_TYPE")
facet = facet.map(sns.lineplot, "PHI_GRID_SIZE", "usedLUT", ci=None).add_legend()
facet.fig.set_size_inches(16, 9)
facet.savefig(saveFolder + "/usedLUT.pdf")

plt.show()