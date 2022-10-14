# -*- coding: utf-8 -*-
"""
Created on Wed Oct 12 15:15:20 2022

@author: addor
"""
import numpy as np

dat = datas3txt[1:-1,:].astype(np.float)

temp =dat.copy()

dat[:,1] = temp[:,2].copy()
dat[:,2] = temp[:,1].copy()

a = np.zeros((len(dat),11))
index = np.where(dat[:,1]>25)
a[index,0]=5
index = np.where(dat[:,1]<6)
a[index,0]=5