#!/usr/bin/env python

import os, sys, shutil, string
import time, re

allDatasets = []
dsFile = open('allMCDatasets.txt', 'r')
dsLines = dsFile.readlines()
for line in dsLines:
  if line[0] == '#': continue
  ds = line.strip()
  allDatasets.append(ds)

for dataset in allDatasets:
  command = './prun_submit.py mc %s v2' % dataset
  if len(sys.argv) > 1: command = command + sys.argv[1]
  print ">", command
  os.system(command)
