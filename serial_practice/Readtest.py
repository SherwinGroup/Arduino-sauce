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


for i in range(3):
    ard.write('200')
    #ard.read()
    print i
    if i == 0:
        one = ard.read()
        print one
    elif i == 1:
        two = ard.read()
        print two
    elif i == 2:
        thr = ard.read()
        print thr
ard.close()
one = [int(i) for i in one.split(';')[1:-1]]
two = [int(i) for i in two.split(';')[1:-1]]
thr = [int(i) for i in thr.split(';')[1:-1]]

#pathname = os.path.join('C:\Users\FELLab\Documents\GitHub\Arduino-sauce', 'one')
#np.savetxt(os.path.join('C:\Users\FELLab\Documents\GitHub\Arduino-sauce', 'one.txt'), one)#, fmt='%0.1e')
#np.savetxt(os.path.join('C:\Users\FELLab\Documents\GitHub\Arduino-sauce', 'two.txt'), two)#, fmt='%0.1e')
#np.savetxt(os.path.join('C:\Users\FELLab\Documents\GitHub\Arduino-sauce', 'three.txt'), thr)#, fmt='%0.1e')
plt.plot(np.array(range(len(one))), one)
plt.plot(np.array(range(len(two))), two)
plt.plot(np.array(range(len(thr))), thr)
plt.show()
