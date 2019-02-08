#!/usr/bin/env python

import glob
from lxml import etree as ET

import pandas as pd


def BuildDataframeFromReports(globPattern, outputCSVPath):
  # 1) Get list of files
  # 2) Get parameters
  # 3) Get metrics
  # 4) Build dataframe

  # Get list of files
  files = glob.glob(globPattern)

  PHI_GRID_SIZE_column = []
  ETA_GRID_SIZE_column = []
  PHI_JET_SIZE_column = []
  ETA_JET_SIZE_column = []
  NUMBER_OF_SEEDS_column = []
  minOverallLatency_column = []
  maxOverallLatency_column = []
  usedBRAM_18K_column = []
  usedFF_column = []
  usedLUT_column = []
  usedDSP48E_column = []
  availableBRAM_18K_column = []
  availableFF_column = []
  availableLUT_column = []
  availableDSP48E_column = []

  # Open XML
  for path in files:
    parser = ET.XMLParser(remove_blank_text=True)
    xmlTree = ET.parse(path, parser)
    rootElement = xmlTree.getroot()
    
    # Get parameters (indices)
    PHI_GRID_SIZE = int(rootElement.find("PHI_GRID_SIZE").text)
    ETA_GRID_SIZE = int(rootElement.find("ETA_GRID_SIZE").text)
    PHI_JET_SIZE = int(rootElement.find("PHI_JET_SIZE").text)
    ETA_JET_SIZE = int(rootElement.find("ETA_JET_SIZE").text)
    NUMBER_OF_SEEDS = int(rootElement.find("NUMBER_OF_SEEDS").text)

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

    PHI_GRID_SIZE_column.append(PHI_GRID_SIZE)
    ETA_GRID_SIZE_column.append(ETA_GRID_SIZE)
    PHI_JET_SIZE_column.append(PHI_JET_SIZE)
    ETA_JET_SIZE_column.append(ETA_JET_SIZE)
    NUMBER_OF_SEEDS_column.append(NUMBER_OF_SEEDS)
    minOverallLatency_column.append(minOverallLatency)
    maxOverallLatency_column.append(maxOverallLatency)
    usedBRAM_18K_column.append(usedBRAM_18K)
    usedFF_column.append(usedFF)
    usedLUT_column.append(usedLUT)
    usedDSP48E_column.append(usedDSP48E)
    availableBRAM_18K_column.append(availableBRAM_18K)
    availableFF_column.append(availableFF)
    availableLUT_column.append(availableLUT)
    availableDSP48E_column.append(availableDSP48E)

  dataframe = pd.DataFrame({
    "PHI_GRID_SIZE": PHI_GRID_SIZE_column,
    "ETA_GRID_SIZE": ETA_GRID_SIZE_column,
    "PHI_JET_SIZE": PHI_JET_SIZE_column,
    "ETA_JET_SIZE": ETA_JET_SIZE_column,
    "NUMBER_OF_SEEDS": NUMBER_OF_SEEDS_column,
    "minOverallLatency": minOverallLatency_column,
    "maxOverallLatency": maxOverallLatency_column,
    "usedBRAM_18K": usedBRAM_18K_column,
    "usedFF": usedFF_column,
    "usedLUT": usedLUT_column,
    "usedDSP48E": usedDSP48E_column,
    "availableBRAM_18K": availableBRAM_18K_column,
    "availableFF": availableFF_column,
    "availableLUT": availableLUT_column,
    "availableDSP48E": availableDSP48E_column,
  })

  dataframe = dataframe.set_index([
    "PHI_GRID_SIZE",
    "ETA_GRID_SIZE",
    "PHI_JET_SIZE",
    "ETA_JET_SIZE",
    "NUMBER_OF_SEEDS",
  ])

  dataframe.to_csv(outputCSVPath)