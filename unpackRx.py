#!/usr/bin/env python

from __future__ import print_function
import os
import sys
import math
import numpy

ptLSB = 1#0.25
etaLSB = 1#0.0043633231
phiLSB = 1#0.0043633231
data = []
zeros = '0000000000000000'
frameIt = -1
wCount = -1

with open("patterns/pattern.txt_0.txt", "r") as inFile:
  for line in inFile:
    if('1v' in line):
      frameIt += 1
      linkData = line.split('1v')
      frameData = []
      if zeros not in linkData[1]: 
        wCount +=1 
      else: 
        wCount = 0
      for wordIt in range(1,25):
        word = linkData[wordIt].replace(' ','')
        frameData.append(word) if int(word, 16) & 0xffff else 0
        #frameData.append(word) if word not in zeros else 0
      data.append(frameData) if frameData else 0
    data.append('\n') if (not (frameIt-1)%13 and frameIt > 1) else 0

newEvent = False
for region in data:
  if '\n' in region:
    if newEvent:
      print('NEW EVENT')
    else: 
      print(' ')
    newEvent = not newEvent
    continue
  for pfCand in region:
    print(str((int(pfCand, 16) & 0xffff) * ptLSB)[:6] +
          '\t' + str(((int(pfCand, 16) >> 26) & 0x3ff) * etaLSB)[:4] + 
          '\t' + str(((int(pfCand, 16) >> 16) & 0x3ff) * phiLSB)[:4] )
