import pyublas
import numpy as np
import math
import STFT

x = np.sin(np.asarray(range(1000)) * 2. * np.pi /10.)
print(x)
y, p = STFT.stft(x, 512, 256)
print(y)
print(p)
