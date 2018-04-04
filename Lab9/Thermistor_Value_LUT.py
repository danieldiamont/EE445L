#Daniel Diamont
#EE445L

#This file was made for the purposes of generating a thermistor bit to temperature LUT

#import numpy and matlab plotting capabilities

import math as m
import numpy as np
import matplotlib.pyplot as plt
import decimal as Decimal

T0 = 25
beta = 3357.55

wav = []
LUT = []

for i in range(0,4095):
        y = beta/(np.log((4096-i)/2817)) + T0
        wav.append(y)
        LUT.append(int(y))

#plot of sine wave
print(LUT);
plt.plot(wav);
plt.show();
