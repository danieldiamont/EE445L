#Daniel Diamont
#EE445L

#This file was made for the purposes of generating a thermistor bit to temperature LUT

#import numpy and matlab plotting capabilities

import math
import numpy as np
import matplotlib.pyplot as plt

Fs = 1024
f = 5
sample = 4096
x = np.arange(sample)
y = 1024*np.sin(2*np.pi*f*x/Fs)+1024
wav = []

for i in range(0,y.size-1):
    wav.append(int(y[i])*np.exp(-i/1024))

#plot of sine wave
plt.plot(wav);
plt.show();
