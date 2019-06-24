#!/usr/bin/env python

'''
Build a dataframe with a few the performance report data and saves it to CSV for later analysis.
'''

import glob
from lxml import etree as ET

import pandas as pd


def BuildDataframeFromReports(globPattern, outputCSVPath, parameterNames):
  # 1) Get list of files
  # 2) Get parameters
  # 3) Get metrics
  # 4) Build dataframe

  # Get list of files
  files = glob.glob(globPattern)

  columns = {}

  for name in parameterNames:
    columns[name] = []

  columns["minOverallLatency"] = []
  columns["maxOverallLatency"] = []
  columns["usedBRAM_18K"] = []
  columns["usedFF"] = []
  columns["usedLUT"] = []
  columns["usedDSP48E"] = []
  columns["availableBRAM_18K"] = []
  columns["availableFF"] = []
  columns["availableLUT"] = []
  columns["availableDSP48E"] = []

  # Open XML
  for path in files:
    parser = ET.XMLParser(remove_blank_text=True)
    xmlTree = ET.parse(path, parser)
    rootElement = xmlTree.getroot()
    
    # Get parameters (indices)

    parameterValues = {}
    for name in parameterNames:
      parameterValues[name] = int(rootElement.find(name).text)

    # Get metrics (columns)

    minOverallLatency = int(rootElement.find("PerformanceEstimates").find("SummaryOfOverallLatency").find("Best-caseLatency").text)
    maxOverallLatency = int(rootElement.find("PerformanceEstimates").find("SummaryOfOverallLatency").find("Worst-caseLatency").text)
    usedBRAM_18K = int(rootElement.find("AreaEstimates").find("Resources").find("BRAM_18K").text)
    usedFF = int(rootElement.find("AreaEstimates").find("Resources").find("FF").text)
    usedLUT = int(rootElement.find("AreaEstimates").find("Resources").find("LUT").text)
    usedDSP48E = int(rootElement.find("AreaEstimates").find("Resources").find("DSP48E").text)
    availableBRAM_18K = int(rootElement.find("AreaEstimates").find("AvailableResources").find("BRAM_18K").text)
    availableFF = int(rootElement.find("AreaEstimates").find("AvailableResources").find("FF").text)
    availableLUT = int(rootElement.find("AreaEstimates").find("AvailableResources").find("LUT").text)
    availableDSP48E = int(rootElement.find("AreaEstimates").find("AvailableResources").find("DSP48E").text)

    for name in parameterNames:
      columns[name].append(parameterValues[name])
    
    columns["minOverallLatency"].append(minOverallLatency)
    columns["maxOverallLatency"].append(maxOverallLatency)
    columns["usedBRAM_18K"].append(usedBRAM_18K)
    columns["usedFF"].append(usedFF)
    columns["usedLUT"].append(usedLUT)
    columns["usedDSP48E"].append(usedDSP48E)
    columns["availableBRAM_18K"].append(availableBRAM_18K)
    columns["availableFF"].append(availableFF)
    columns["availableLUT"].append(availableLUT)
    columns["availableDSP48E"].append(availableDSP48E)

  dataframe = pd.DataFrame(columns)
  dataframe.to_csv(outputCSVPath)