import pyublas
import numpy as np
import math
import STFT
import matplotlib.pyplot as plt

t = np.asarray(range(160000)) / 16000.
x = np.sin(1650. * t * 2. * np.pi) + np.sin(2200. * t * 2. * np.pi) / 2. + np.sin(4400. * t * 2. * np.pi) / 5.
print(x)
plt.plot(t, x)
plt.show()

y, p = STFT.stft(x, 512, 256)
print(y)
plt.imshow(y, interpolation='nearest', aspect='auto')
plt.show()

print(p)
plt.imshow(p, interpolation='nearest', aspect='auto')
plt.show()
