#! /usr/bin/env python

'''
Utility script that scans the parameter space to analyse the FW performance.
1) replaces the #define in the header to set the value of interest
2) run synthesis
3) retrieves the XML containing the report and saves it in a folder
4) extracts a few parameters of interest from the report and puts them in a pandas dataframe
'''

from tempfile import mkstemp
from shutil import move
from os import fdopen, remove, system, WEXITSTATUS
from itertools import product
from lxml import etree as ET
from BuildDataframeReport import BuildDataframeFromReports


parameters = {}

parameters["PHI_GRID_SIZE"] = 1
parameters["ETA_GRID_SIZE"] = 2
parameters["PHI_JET_SIZE"] = 3
parameters["ETA_JET_SIZE"] = 4
parameters["NUMBER_OF_SEEDS"] = 5

values = {}
values["PHI_GRID_SIZE"] = [9, 45, 72]
values["ETA_GRID_SIZE"] = [9, 45, 82]
values["PHI_JET_SIZE"] = [3, 5, 7, 9]
values["ETA_JET_SIZE"] = [3, 5, 7, 9]
values["NUMBER_OF_SEEDS"] = [64, 128]

saveFolder = "reports"

def buildHeader(file_path, parameters):
  #Create temp file
  fh, abs_path = mkstemp()
  with fdopen(fh,'w') as new_file:
    with open(file_path) as old_file:
      for line in old_file:
        if (line.find("define") == True):
          parsed_line = line.split()
          if parsed_line[1] in parameters:
            parsed_line[2] = str(parameters[parsed_line[1]])
          new_file.write(" ".join(parsed_line) + "\n")
        else:
          new_file.write(line)
  #Remove original file
  remove(file_path)
  #Move new file
  move(abs_path, file_path)

def runVivadoTCL(tclPath):
  # running vivado_hls
  returnValue = system("vivado_hls FPGAClustering_Synthetise.tcl")
  # checking if return value != 0 to look for errors
  if WEXITSTATUS(returnValue) != 0:
    raise RuntimeError("vivado_hls returned value " + WEXITSTATUS(returnValue) + "check log and latest header for more info. Aborting scan.")

def addParametersAndStore(inputFile, parameters, outputFile):
  # opening the file
  parser = ET.XMLParser(remove_blank_text=True)
  xmlTree = ET.parse(inputFile, parser)
  # adding the parameters for logging
  rootElement = xmlTree.getroot()
  for parName in parameters:
    subElement = ET.SubElement(rootElement, parName)
    subElement.text = str(parameters[parName])
  # saving the XML file
  xmlTree.write(outputFile, pretty_print=True)

if __name__ == "__main__":
  
  for combination in product(values["PHI_GRID_SIZE"], values["ETA_GRID_SIZE"], values["PHI_JET_SIZE"], values["ETA_JET_SIZE"], values["NUMBER_OF_SEEDS"]):
    parameters["PHI_GRID_SIZE"] = combination[0]
    parameters["ETA_GRID_SIZE"] = combination[1]
    parameters["PHI_JET_SIZE"] = combination[2]
    parameters["ETA_JET_SIZE"] = combination[3]
    parameters["NUMBER_OF_SEEDS"] = combination[4]
    buildHeader("HLS_Phase1Clustering.h", parameters)
    runVivadoTCL("FPGAClustering_Synthetise.tcl")
    addParametersAndStore(
      "FPGAClustering/FPGAClustering/syn/report/hls_main_csynth.xml",
      parameters,
      saveFolder + "/hls_main_csynth"
      + "_" + str(combination[0])
      + "_" + str(combination[1])
      + "_" + str(combination[2])
      + "_" + str(combination[3])
      + "_" + str(combination[4])
      + ".xml"
    )
    addParametersAndStore(
      "FPGAClustering/FPGAClustering/syn/report/buildJets_csynth.xml",
      parameters,
      saveFolder + "/buildJets_csynth"
      + "_" + str(combination[0])
      + "_" + str(combination[1])
      + "_" + str(combination[2])
      + "_" + str(combination[3])
      + "_" + str(combination[4])
      + ".xml"
    )
  
  BuildDataframeFromReports(saveFolder + "/hls_main_csynth*.xml", saveFolder + "/hls_main_csynth_dataframe.csv")
  BuildDataframeFromReports(saveFolder + "/buildJets_csynth*.xml", saveFolder + "/buildJets_csynth_dataframe.csv")