import math
import numpy as np

wave = [
  2048*2,2448*2,2832*2,3186*2,3496*2,3751*2,3940*2,4057*2,4095*2,4057*2,3940*2,
  3751*2,3496*2,3186*2,2832*2,2448*2,2048*2,1648*2,1264*2,910*2,600*2,345*2,
  156*2,39*2,0*2,39*2,156*2,345*2,600*2,910*2,1264*2,1648*2]

new_wave = []

for i in range(0,31):
    new_wave.append((int)(wave[i]*math.exp(-i/8)))

Fs = 256
f = 5
sample = 256
x = np.arange(sample)
y = 2048*np.cos(2*np.pi*f*x/Fs)+2048
wav = []
for i in range(0,y.size-1):
    wav.append(int(y[i]*math.exp(-i/256)))

print(wav)
print('size: ' + str(y.size))
