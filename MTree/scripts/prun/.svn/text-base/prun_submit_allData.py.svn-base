#!/usr/bin/env python

import os, sys, shutil, string
import time, re

inputFile = 'EgammaSUSYD3PD000801.r16.periodH.txt'
suffix    = 'ver0.1'

allDatasets = []
dsFile = open('%s' % inputFile, 'r')
dsLines = dsFile.readlines()
for line in dsLines:
  if line[0] == '#': continue
  ds = line.strip()
  allDatasets.append(ds)

for dataset in allDatasets:
  command = './prun_submit.py data %s %s' % (dataset, suffix)
  if len(sys.argv) > 1: command = command + sys.argv[1]
  print ">", command
  os.system(command)
