#Daniel Diamont
#EE445L

#This file was made for the purposes of generating a thermistor bit to temperature LUT

#import numpy and matlab plotting capabilities

import math as m
import numpy as np
import matplotlib.pyplot as plt

wav = []
LUT = []

B = 3357.55;
V0 = 0.163;
T0 = 298.15;
R = 500e3;
R0 = 104.5e3;

Vt = 0.004
step = 0.233/4096

denom = R0*np.exp(-B/T0);

for i in range(0,4095):
        y = (B/(np.log(((R*((Vt + (step*i) + V0)/2.5))/(1 - ((Vt + (step*i) + V0)/2.5))/denom)))) - 273.15 - 7.48
        wav.append(int(y*100))

#plot of sine wave
print(wav);
plt.plot(wav);
plt.show();
