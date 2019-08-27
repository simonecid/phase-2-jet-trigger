#!/usr/bin/env python


def packPfInput(pt, eta, phi):
  jetValue = pt + (phi << 16) + (eta << 26)
  hexJetValue = hex(jetValue)
  packedWord = "1v0000000000000000"
  # remove 0x from jet, count how many character, remove that number of characters from the tail of the base word and replace them with the jet
  packedWord = packedWord[:-len(hexJetValue[2:])] + hexJetValue[2:]
  return packedWord

import numpy as np

# numberOfSamples = 256
numberOfSamples = 24

fileContent = []

for generation in range(0, 10):

  # generates grid of random numbers
  pt_array = np.random.randint(0, 30, size=numberOfSamples)
  eta_array = np.random.randint(0, 171, size=numberOfSamples)
  phi_array = np.random.randint(0, 160, size=numberOfSamples)
  #event = np.random.normal(loc=30, scale=10, size=(72, 96))
  #event = np.full((72, 96), 1)
  
  event = np.empty((numberOfSamples, 3))

  line = str(generation) + ": "

  for x in range(0, len(pt_array)):
    packedData = packPfInput(pt_array[x], eta_array[x], phi_array[x])
    line += packedData
    line += " " if x != len(pt_array) - 1 else "\n"
    event[x][0] = pt_array[x]
    event[x][1] = eta_array[x]
    event[x][2] = phi_array[x]

  # save event to file
  np.savetxt("inputs_" + str(generation)+ ".dat", event, fmt="%u")
  
  fileContent.append(line)

    

  #bin stuff
  bins, x_edges, y_edges = np.histogram2d(eta_array, phi_array, bins=[9, 8], range=[[0, 171], [0, 160]], weights=pt_array)
  #bins is in [x][y] format, i want [y][x]
  bins = np.transpose(bins)
  np.savetxt("inputs_histogram_" + str(generation) + ".dat", bins, fmt="%u")

outputFile = open("inputs.dat", "w")
outputFile.writelines(fileContent)
outputFile.close()