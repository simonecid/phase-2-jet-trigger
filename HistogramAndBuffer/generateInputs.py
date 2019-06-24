#!/usr/bin/env python

import numpy as np

numberOfSamples = 256

# generates grid of random numbers
pt_array = np.random.randint(0, 10, size=numberOfSamples)
eta_array = np.random.randint(0, 100, size=numberOfSamples)
phi_array = np.random.randint(0, 100, size=numberOfSamples)
#event = np.random.normal(loc=30, scale=10, size=(72, 96))
#event = np.full((72, 96), 1)

event = np.empty((numberOfSamples, 3))

for x in xrange(0, len(pt_array)):
  event[x][0] = pt_array[x]
  event[x][1] = eta_array[x]
  event[x][2] = phi_array[x]

# save event to file
np.savetxt("inputs.dat", event, fmt="%u")

#bin stuff
bins, x_edges, y_edges = np.histogram2d(eta_array, phi_array, bins=10, range=[[0, 100], [0, 100]], weights=pt_array)
np.savetxt("inputs_histogram.dat", bins, fmt="%u")