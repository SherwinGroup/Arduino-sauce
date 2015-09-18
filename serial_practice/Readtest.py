# -*- coding: utf-8 -*-
"""
Created on Tue Sep 15 15:05:58 2015

@author: hbanks

Brevity required, prurience preferred
"""

from __future__ import division
import os
import glob
import numpy as np
import matplotlib.pyplot as plt
import time
import visa

plt.close()
rm = visa.ResourceManager()
print rm.list_resources()
ard = rm.open_resource(u'ASRL4::INSTR')
ard.baud_rate = 115200
ard.timeout = 10000


for i in range(50):
    ard.read()
    print i
    if i == 1:
        one = ard.read()
        print one
    elif i == 25:
        two = ard.read()
        print two
    elif i == 49:
        thr = ard.read()
        print thr
ard.close()
one = [int(i) for i in one.split(';')[1:-1]]
two = [int(i) for i in two.split(';')[1:-1]]
thr = [int(i) for i in thr.split(';')[1:-1]]

plt.plot(np.array(range(len(one))), one)
plt.plot(np.array(range(len(two))), two)
plt.plot(np.array(range(len(thr))), thr)
plt.show()
