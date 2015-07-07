#! /usr/bin/env python

import os
import sys
import fileinput
import string

set1 = []
set2 = []

input1 = open("check1.txt","r")
lines1 = input1.readlines()
for line in lines1:
    set1.append(line[21:32])

input2 = open("check2.txt","r")
lines2 = input2.readlines()
for line in lines2:
    set2.append(line[11:22])

for jot in set2:
    if jot not in set1:
        print jot


